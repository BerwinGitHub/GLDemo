//
//  SmearRGBA.hpp
//  GLDemo
//
//  Created by 唐博文 on 16/8/13.
//
//

#ifndef SmearRGBA_hpp
#define SmearRGBA_hpp

#include "SmearNode.hpp"

typedef Sprite Shape;   // smear shape

extern CC_DLL const GLchar *SmearRGBAPositonTextureColor_frag;
#define kSmearRGBAShaderName "SmearRGBAShaderName"

class SmearRGBA : public SmearNode {
    
public:
    SmearRGBA();
    virtual ~SmearRGBA();
    
public:
    static SmearRGBA *createWithRGBA(const Color4B &c);
    virtual bool initWithRGBA(const Color4B &c);
    
public:
    virtual void setTarget(Texture2D* tex) override;
    virtual void setTargetColor(const Color4F &c);
    virtual void setTargetColor(const Color4B &c);
    
public:
    virtual void initShader() override;
    
public:
    virtual void onEnter() override;
    virtual void onExit() override;
    
private:
    void bindShapeTexture();
    
};

#endif /* SmearRGBA_hpp */
