
#include "StackBlur.h"

/**
 * 自定义一个线程类.
 */
class Thread {
    
public:
    Thread(std::string name = "Unknown"){}
    virtual ~Thread(){}
    
public:
    void start(){
        pthread_create(&m_ThreadID, nullptr, ThreadFunc, this);
    }
    void wait(){
        pthread_join(m_ThreadID, nullptr);
    }
    static void* ThreadFunc(void* pth){
        Thread* p = static_cast<Thread*>(pth);
        p->run();
        return nullptr;
    }
    virtual void run() = 0;
    
private:
    pthread_t m_ThreadID;
};


/**
 * 异步线程去处理模糊计算
 */
class MVImageUtilsStackBlurTask : public Thread {
public:
    unsigned char* src;
    unsigned int w;
    unsigned int h;
    unsigned int radius;
    int cores;
    int core;
    int step;
    unsigned char* stack;
    
    inline MVImageUtilsStackBlurTask(unsigned char* src, unsigned int w, unsigned int h, unsigned int radius, int cores, int core, int step, unsigned char* stack) {
        this->src = src;
        this->w = w;
        this->h = h;
        this->radius = radius;
        this->cores = cores;
        this->core = core;
        this->step = step;
        this->stack = stack;
    }
    
    // 主要是异步线程去模糊图片
    inline void run() {
        stackblurJob(src, w, h, radius, cores, core, step, stack);
    }
    
    
};


/// Stackblur algorithm by Mario Klingemann
/// Details here:
/// http://www.quasimondo.com/StackBlurForCanvas/StackBlurDemo.html
/// C++ implemenation base from:
/// https://gist.github.com/benjamin9999/3809142
/// http://www.antigrain.com/__code/include/agg_blur.h.html
/// This version works only with RGBA color
inline void 			   stackblur(unsigned char* src,				///< input image data
                             unsigned int w,					///< image width
                             unsigned int h,					///< image height
                             unsigned int radius,				///< blur intensity (should be in 2..254 range)
                             int cores = 1						///< number of threads (1 - normal single thread)
){
    if (radius > 254) return;
    if (radius < 2) return;
    
    unsigned int div = (radius << 1) + 1;
    unsigned char* stack = new unsigned char [(div << 2) * cores];
    
    if (cores == 1) {
        // no multithreading
        stackblurJob(src, w, h, radius, 1, 0, 1, stack);
        stackblurJob(src, w, h, radius, 1, 0, 2, stack);
    } else{
        MVImageUtilsStackBlurTask** workers = new MVImageUtilsStackBlurTask*[cores];
        for (int i = 0; i < cores; i++) {
            workers[i] = new MVImageUtilsStackBlurTask(src, w, h, radius, cores, i, 1, stack + (div << 2) * i);
            workers[i]->start();
        }
        
        for (int i = 0; i < cores; i++)
            workers[i]->wait();
        
        for (int i = 0; i < cores; i++)  {
            workers[i]->step = 2;
            workers[i]->start();
        }
        
        for (int i = 0; i < cores; i++) {
            workers[i]->wait();
            delete workers[i];
        }
        
        delete[] workers;
    }
    delete[] stack;
}