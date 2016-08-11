//
//  SmearColor.cpp
//  GLDemo
//
//  Created by tangbowen on 16/8/11.
//
//

#include "SmearColor.hpp"
#include "./shader/SmearColorShaderFrag.frag"
#include "./support/GLProgramUtility.hpp"

SmearColor::SmearColor()
: _pShape(nullptr)
{
    
}

SmearColor::~SmearColor()
{
    
}

SmearColor* SmearColor::createWithShape(Shape *s)
{
    SmearColor *pRet = new SmearColor();
    if (pRet && pRet->initWithShape(s)) {
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_RELEASE_NULL(pRet);
    return nullptr;
}

bool SmearColor::initWithShape(Shape *s)
{
    if (!SmearNode::init(Target::create(), Paint::create())) {
        return false;
    }
    this->setShape(s);
    
    return true;
}

void SmearColor::initShader()
{
    string key = GLUtility::generatedUniqueGLProgramKey(kSmearColorShaderName, this);
    GLProgram *p = GLUtility::loadGLProgram(ccPositionTextureColor_noMVP_vert, smearColorPositonTextureColor_frag, key);
    if (p && _pPaint->getGLProgram() != p) {
        this->setGLProgram(_pPaint, p);
        
        this->bindPaintAlphaValue(0.0f);
        this->bindTargetAlphaValue(0.0f);
        this->setPaintHardness(1.0f);
        this->bindPaintTexture();
    }
}

void SmearColor::setShape(cocos2d::Sprite *shape)
{
    if (!_pShape && _pShape->getTexture() == shape->getTexture())
        return;
    if (!_pShape) {
        _pShape = shape;
        _pShape->retain();
    } else {
        shape->retain();
        _pShape->release();
        _pShape = shape;
    }
    this->bindShapeTexture();
}

void SmearColor::bindShapeTexture()
{
    GLUtility::bindUniformVec2(_pPaint, "v_texSize_shape", _pShape->getContentSize());
    GLUtility::bindUniformTexture(_pPaint, "s_texture_shape", _pShape->getTexture());
}

void SmearColor::setTargetTexture(cocos2d::Texture2D *tex)
{
    SmearNode::setTargetTexture(tex);
    GLUtility::bindUniformInt(_pPaint, "v_taret_type_color", 0);
}

void SmearColor::setTargetColor(const cocos2d::Color4F &c)
{
    GLUtility::bindUniformInt(_pPaint, "v_taret_type_color", 1);
    GLUtility::bindUniformVec4(_pPaint, "v_color_target", Vec4(c.r, c.g, c.b, c.a));
}

void SmearColor::setTargetColor(const cocos2d::Color4B &c)
{
    this->setTargetColor(Color4F(c));
}

void SmearColor::onEnter()
{
    SmearNode::onEnter();
}

void SmearColor::onExit()
{
    SmearNode::onExit();
}
