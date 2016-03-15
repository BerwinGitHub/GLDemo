//
//  HSLSprite.h
//  Demo
//
//  Created by tangbowen on 10/3/16.
//
//

#ifndef HSLSprite_hpp
#define HSLSprite_hpp

#include "cocos2d.h"
#include "../support/GLProgramUnifos.h"

USING_NS_CC;
using namespace std;

class HSLSprite : public Sprite{
    
public:
    typedef struct _HSL{
        float h;// Hue          色调(-180.0f ~ 180.0f)
        float s;// Stauration   饱和度(-1.0f ~ 1.0f)
        float l;// Lightness    饱和度(-1.0f ~ 1.0f)
    } HSL;
    
public:
    HSLSprite();
    virtual ~HSLSprite();
    
public:
    static HSLSprite* createWithHSL(const string &file, HSLSprite::HSL hsl);
    virtual bool initWithHSL(const string &file, HSLSprite::HSL hsl);
    
    void setHue(float h);
    void setStauration(float s);
    void setLightness(float l);
    void setHSL(float h, float s, float l);// 都为0 表示不改变他的颜色
    
protected:
    virtual void initShaderProgram();
    
protected:
    virtual void draw(cocos2d::Renderer *renderer,const cocos2d::Mat4& transform,uint32_t flags) override;
    virtual void onEnter() override;
    virtual void onExit() override;
    
private:
    GLProgramUnifos    *_pAdjustHSLUnifos;
    CC_SYNTHESIZE(HSL, _sHsl, HSL);
};

#endif /* HSLSprite_hpp */
