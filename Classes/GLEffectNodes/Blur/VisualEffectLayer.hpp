//
//  VisualEffectLayer.hpp
//  Follow
//
//  Created by 唐博文 on 15/12/30.
//
//

#ifndef VisualEffectLayer_hpp
#define VisualEffectLayer_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "StackBlurTask.h"

USING_NS_CC;

class VisualEffectLayer : public Node{
    
public:
    VisualEffectLayer();
    ~VisualEffectLayer();
    LayerColor* s;
    
public:
    static VisualEffectLayer* createWithRadius(const Size &size, int radius);
    bool initWithRadius(const Size &size, int radius);
    
protected:
    CustomCommand _customCommand;
    virtual void draw(cocos2d::Renderer *renderer,const cocos2d::Mat4& transform,uint32_t flags) override;
    void onDraw(const Mat4& transform, uint32_t flags);
    
//    void prepareCommand();
//    void onCaptureScreen(const std::function<void(bool, std::string)>, const string &file);
    
protected:
    int   _iFrameRate;
    virtual void update(float dt) override;
    
    void captureFinish(unsigned char *data, int w, int h);
    
private:
    Sprite* _pBlurSprite;
    float   _fRadius;
    
public:
    virtual void onEnter() override;
    virtual void onExit() override;
    
};

void onCaptureScreen(const std::function<void (unsigned char*, int w, int h)>, Rect &bb);


#endif /* VisualEffectLayer_hpp */
