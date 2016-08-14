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

typedef Sprite TargetShape;   // need a spite to know size

extern CC_DLL const GLchar *SmearRGBAPositonTextureColor_frag;
#define kSmearRGBAShaderName "SmearRGBAShaderName"

class SmearRGBA : public SmearNode {
    
public:
    SmearRGBA();
    virtual ~SmearRGBA();
    
public:
    // TargetSize need a spite to know size
    static SmearRGBA *createWithRGBA(TargetShape *s, const Color4B &c);
    static SmearRGBA *createWithRGBA(const string &tsFile, const Color4B &c);
    virtual bool initWithRGBA(TargetShape *s, const Color4B &c);
    
public:
    virtual void setTarget(Texture2D* tex) override;
    virtual void setTargetColor(const Color4F &c);
    virtual void setTargetColor(const Color4B &c);
    
    virtual void setReverse(bool reverse);
    virtual bool isReverse();
    
public:
    virtual void initShader() override;
    
public:
    virtual void onEnter() override;
    virtual void onExit() override;
    
private:
    void bindShapeTexture();
    
private:
    bool    _bReverse;
    
};

#endif /* SmearRGBA_hpp */
