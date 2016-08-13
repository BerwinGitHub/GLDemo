//
//  SmearRGBA.cpp
//  GLDemo
//
//  Created by 唐博文 on 16/8/13.
//
//

#include "SmearRGBA.hpp"
#include "./shader/SmearRGBAShaderFrag.frag"
#include "./support/GLProgramUtility.hpp"

SmearRGBA::SmearRGBA()
{
    
}

SmearRGBA::~SmearRGBA()
{
    
}

SmearRGBA* SmearRGBA::createWithRGBA(const cocos2d::Color4B &c)
{
    SmearRGBA *pRet = new SmearRGBA();
    if (pRet && pRet->initWithRGBA(c)) {
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_RELEASE_NULL(pRet);
    return nullptr;
}

bool SmearRGBA::initWithRGBA(const cocos2d::Color4B &c)
{
    if (!SmearNode::init(Target::create("images/scrrible_rect.png"), Paint::create())) {
        return false;
    }
    this->setTargetColor(c);
    this->bindShapeTexture();
    
    return true;
}

void SmearRGBA::initShader()
{
    string key = GLUtility::generatedUniqueGLProgramKey(kSmearRGBAShaderName, this);
    GLProgram *p = GLUtility::loadGLProgram(ccPositionTextureColor_noMVP_vert, SmearRGBAPositonTextureColor_frag, key);
    if (p && _pPaint->getGLProgram() != p) {
        this->setGLProgram(_pPaint, p);
        
        this->bindPaintAlphaValue(0.0f);
        this->bindTargetAlphaValue(0.0f);
        this->setPaintHardness(1.0f);
        this->bindPaintTexture();
    }
}

void SmearRGBA::bindShapeTexture()
{
    GLUtility::bindUniformVec2(_pPaint, "v_texSize_shape", _pTarget->getContentSize());
    GLUtility::bindUniformTexture(_pPaint, "s_texture_shape", _pTarget->getTexture());
}

void SmearRGBA::setTarget(cocos2d::Texture2D *tex)
{
    SmearNode::setTarget(tex);
    GLUtility::bindUniformInt(_pPaint, "v_taret_type_color", 0);
}

void SmearRGBA::setTargetColor(const cocos2d::Color4F &c)
{
    GLUtility::bindUniformInt(_pPaint, "v_taret_type_color", 1);
    GLUtility::bindUniformVec4(_pPaint, "v_color_target", Vec4(c.r, c.g, c.b, c.a));
}

void SmearRGBA::setTargetColor(const cocos2d::Color4B &c)
{
    this->setTargetColor(Color4F(c));
}

void SmearRGBA::onEnter()
{
    SmearNode::onEnter();
}

void SmearRGBA::onExit()
{
    SmearNode::onExit();
}
