//
//  SmearColor.hpp
//  GLDemo
//
//  Created by tangbowen on 16/8/11.
//
//

#ifndef SmearColor_hpp
#define SmearColor_hpp

#include "SmearNode.hpp"

typedef Sprite Shape;   // smear shape

extern CC_DLL const GLchar *smearColorPositonTextureColor_frag;
#define kSmearColorShaderName "SmearColorShaderName"

class SmearColor : public SmearNode {
    
public:
    SmearColor();
    virtual ~SmearColor();
    
public:
    static SmearColor *createWithShape(Shape *s);
    virtual bool initWithShape(Shape *s);
    
public:
    void setShape(Sprite *shape);
    void bindShapeTexture();
    
    virtual void setTargetTexture(Texture2D* tex) override;
    virtual void setTargetColor(const Color4F &c);
    virtual void setTargetColor(const Color4B &c);
    
public:
    virtual void initShader() override;
    
protected:
    Shape  *_pShape;
    
public:
    virtual void onEnter() override;
    virtual void onExit() override;
    
};

#endif /* SmearColor_hpp */
