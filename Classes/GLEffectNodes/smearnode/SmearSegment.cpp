//
//  SmearSegment.cpp
//  GLDemo
//
//  Created by 唐博文 on 16/8/13.
//
//

#include "SmearSegment.hpp"
#include "./shader/SmearSegmentShaderFrag.frag"
#include "./support/GLProgramUtility.hpp"

SmearSegment::SmearSegment()
: _pImage(nullptr)
, _pSegment(nullptr)
{
    
}

SmearSegment::~SmearSegment()
{
    CC_SAFE_RELEASE_NULL(_pImage);
    CC_SAFE_RELEASE_NULL(_pSegment);
}

SmearSegment* SmearSegment::createWithSegment(const string &segment, const string &target)
{
    SmearSegment *pRet = new SmearSegment();
    if (pRet && pRet->initWithSegment(segment, target)) {
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_RELEASE_NULL(pRet);
    return nullptr;
}

bool SmearSegment::initWithSegment(const string &segment, const string &target)
{
    if (!SmearNode::init(Target::create(target), Paint::create())) {
        return false;
    }
    this->setSegment(segment);
    
    _pImage = new Image();
    _pImage->initWithImageFile(segment);
    
    this->getSprite()->setBlendFunc(BlendFunc::ALPHA_PREMULTIPLIED);
    
    return true;
}

void SmearSegment::initShader()
{
    string key = GLUtility::generatedUniqueGLProgramKey(kSmearSegmentShaderName, this);
    GLProgram *p = GLUtility::loadGLProgram(ccPositionTextureColor_noMVP_vert, SmearSegmentPositonTextureColor_frag, key);
    if (p && _pPaint->getGLProgram() != p) {
        this->setGLProgram(_pPaint, p);
        
        this->bindPaintAlphaValue(0.0f);
        this->bindTargetAlphaValue(0.0f);
        this->setPaintHardness(1.0f);
        this->bindPaintTexture();
    }
}

void SmearSegment::setSegment(const string &file)
{
    CC_SAFE_RELEASE_NULL(_pSegment);
    _pSegment = Segment::create(file);
    _pSegment->setPosition(_pTarget->getContentSize() * 0.5f);
    _pSegment->retain();
    this->bindSegmentTexture();// bind target size
    
    CC_SAFE_RELEASE_NULL(_pImage);
    _pImage = new Image();
    _pImage->initWithImageFile(file);
}

void SmearSegment::setSegmentColor(const cocos2d::Color4F &c)
{
    this->bindSegmentColor(c);
}

void SmearSegment::bindSegmentTexture()
{
    Texture2D::TexParams tp;
    tp.magFilter = GL_NEAREST;
    tp.minFilter = GL_NEAREST;
    tp.wrapS = GL_CLAMP_TO_EDGE;
    tp.wrapT = GL_CLAMP_TO_EDGE;
    _pSegment->getTexture()->setTexParameters(tp);
    
    GLUtility::bindUniformVec2(_pPaint, "v_texSize_segment", _pSegment->getContentSize());
    GLUtility::bindUniformTexture(_pPaint, "s_texture_segment", _pSegment->getTexture());
}

void SmearSegment::bindSegmentColor(const cocos2d::Color4F &c)
{
    GLUtility::bindUniformVec4(_pPaint, "v_color_segment", Vec4(c.r, c.g, c.b, c.a));
}

void SmearSegment::setTarget(cocos2d::Texture2D *tex)
{
    SmearNode::setTarget(tex);
    GLUtility::bindUniformInt(_pPaint, "b_color_enable", 0);
}

void SmearSegment::setTargetColor(const cocos2d::Color4F &c)
{
    GLUtility::bindUniformVec4(_pPaint, "v_color_target", Vec4(c.r, c.g, c.b, c.a));
    GLUtility::bindUniformInt(_pPaint, "b_color_enable", 1);
}

void SmearSegment::setTargetColor(const cocos2d::Color4B &c)
{
    this->setTargetColor(Color4F(c));
}

Color4F SmearSegment::getPixelColor(const cocos2d::Vec2 &p)
{
    Color4B c = Color4B::BLACK;
    Vec2 pos = this->getSprite()->convertToNodeSpace(p);
    unsigned int x = pos.x, y = _pImage->getHeight() - pos.y;
    unsigned char *data = _pImage->getData();
    unsigned int *pixel = (unsigned int *)data;
    pixel = pixel + (y * _pImage->getWidth()) + x;
    c.r = *pixel & 0xff;
    c.g = (*pixel >> 8) & 0xff;
    c.b = (*pixel >> 16) & 0xff;
    c.a = (*pixel >> 24) & 0xff;
    return Color4F(c);
}

void SmearSegment::onEnter()
{
    SmearNode::onEnter();
}

void SmearSegment::onExit()
{
    SmearNode::onExit();
}
