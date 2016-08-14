//
//  .hpp
//  GLDemo
//
//  Created by 唐博文 on 16/8/13.
//
//

#ifndef _hpp
#define _hpp

#include "SmearNode.hpp"

typedef Sprite  Segment;

extern CC_DLL const GLchar *SmearSegmentPositonTextureColor_frag;
#define kSmearSegmentShaderName "SmearSegmentShaderName"

class SmearSegment : public SmearNode {
    
public:
    SmearSegment();
    virtual ~SmearSegment();
    
public:
    // TargetSize need a spite to know size
    static SmearSegment *createWithSegment(const string &segment, const string &target);
    virtual bool initWithSegment(const string &segment, const string &target);
    
public:
    Color4F getPixelColor(const Vec2 &p);
    virtual void setSegment(const string &file);
    virtual void setSegmentColor(const Color4F &c);
    
    virtual void setTarget(Texture2D* tex) override;
    virtual void setTargetColor(const Color4F &c);
    virtual void setTargetColor(const Color4B &c);
    
public:
    virtual void initShader() override;
    
public:
    virtual void onEnter() override;
    virtual void onExit() override;
    
private:
    void bindSegmentTexture();
    void bindSegmentColor(const Color4F &c);
    
private:
    Image   *_pImage;
    CC_SYNTHESIZE(Segment*, _pSegment, Segment);
    
};

#endif /* _hpp */
