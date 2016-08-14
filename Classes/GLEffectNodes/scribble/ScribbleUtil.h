//
//  Util.h
//  ScribbleDemo_3_2
//
//  Created by renhong on 4/2/15.
//
//

#ifndef __ScribbleDemo_3_2__Util__
#define __ScribbleDemo_3_2__Util__

#include <stdio.h>
#include <cocos2d.h>
#include <vector>
using namespace cocos2d;
using namespace std;

//namespace Scribble {

    class ScribbleUtil{
    public:
        static ScribbleUtil* getInstance();
        Color4B getColor(Image *pImage, Vec2 pPosition);
        // 判断OpenGL是否支持某个指定的扩展
        // 若支持，返回true。否则返回false。
        inline bool supportGLExtension(const char* name);
        //获取当前时间（毫秒）
        long getCurrentTimeByMicroseconds();
        Image *getImage(Sprite *pSprite);
        vector<Point> interpolate(Point pStartPosition, Point pEndPosition, float pDelta);
        GLProgram *loadGLProgram(const GLchar * pVertProgram, const GLchar *pFragProgram, string pGLProgramKey);
        void setGLProgram(Node *n, GLProgram *p);
        inline string createUniqueGLProgramName(string pGLProgramName, void *pGLProgramCreater){
//            char *lName = new char[pGLProgramName.length() + sizeof(pGLProgramCreater) * 2];
            char lName[512];//anroid 使用new，然后delete会crash，考虑此处长度不会超出512，所以使用局部栈变量。
            sprintf(lName, "%s%p", pGLProgramName.c_str(), pGLProgramCreater);
            string lRes(lName);
//            delete []lName;
            return lRes;
        }

    private:
        static ScribbleUtil *s_SharedUtil;
    };
//}

#endif /* defined(__ScribbleDemo_3_2__Util__) */
