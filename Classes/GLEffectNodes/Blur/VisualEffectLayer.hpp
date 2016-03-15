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
    
    virtual void setPosition(const Vec2 &position) override;
    virtual void setContentSize(const Size& contentSize) override;
    
protected:
    CustomCommand _customCommand;
    virtual void draw(cocos2d::Renderer *renderer,const cocos2d::Mat4& transform,uint32_t flags) override;
    void onDraw(const Mat4& transform, uint32_t flags);
    
    virtual void onEnter() override;
    virtual void onExit() override;
    
protected:
    
protected:
    Rect    _rCapturePixelRect;
    void captureFinish(unsigned char *data, int w, int h);
    
private:
    void transferCapturePixelRect();
    
private:
    float   _fRadius;
    Texture2D*  _pBlurTexture;
    
};

void onCaptureScreen(const std::function<void (unsigned char*, int w, int h)>, Rect &bb);


#endif /* VisualEffectLayer_hpp */
