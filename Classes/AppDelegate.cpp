#include "AppDelegate.h"
#include "ControllerScene.hpp"

USING_NS_CC;

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
        glview = GLView::create("My Game");
        director->setOpenGLView(glview);
    }
    
    this->setUpEnvironment(640, 960);

    // turn on display FPS
    director->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);
    
    
    auto scene = ControllerScene::scene();
    director->runWithScene(scene);

    // run
    director->runWithScene(scene);

    return true;
}

void AppDelegate::setUpEnvironment(float width, float height)
{
    srand((int)time(nullptr));
    
    Size m_sDesignSize = Size(width, height);
    Director* director = Director::getInstance();
    GLView* glview = director->getOpenGLView();
    
    Size frameSize = glview->getFrameSize();
    
    float scaleX = frameSize.width / m_sDesignSize.width;
    float scaleY = frameSize.height / m_sDesignSize.height;
    float scale = MIN(scaleX, scaleY);
    
    Size m_sRealDesignSize = Size(frameSize.width / scale, frameSize.height / scale);
    
    glview->setDesignResolutionSize(m_sRealDesignSize.width, m_sRealDesignSize.height, ResolutionPolicy::NO_BORDER);
    
    // set the offset
    Size winSize = director->getWinSize();
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
