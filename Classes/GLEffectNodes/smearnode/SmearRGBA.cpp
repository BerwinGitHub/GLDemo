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
: _bReverse(false)
{
    
}

SmearRGBA::~SmearRGBA()
{
    
}

SmearRGBA* SmearRGBA::createWithRGBA(TargetShape *s, const cocos2d::Color4B &c)
{
    SmearRGBA *pRet = new SmearRGBA();
    if (pRet && pRet->initWithRGBA(s, c)) {
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_RELEASE_NULL(pRet);
    return nullptr;
}

SmearRGBA* SmearRGBA::createWithRGBA(const string &trFile, const cocos2d::Color4B &c)
{
    return SmearRGBA::createWithRGBA(TargetShape::create(trFile), c);
}

bool SmearRGBA::initWithRGBA(TargetShape *s, const cocos2d::Color4B &c)
{
    if (!SmearNode::init(s, Paint::create())) {
        return false;
    }
    this->setTargetColor(c);
    this->bindShapeTexture();// bind target size
    this->setReverse(false);
    
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
    GLUtility::bindUniformInt(_pPaint, "b_color_enable", 0);
}

void SmearRGBA::setTargetColor(const cocos2d::Color4F &c)
{
    GLUtility::bindUniformInt(_pPaint, "b_color_enable", 1);
    GLUtility::bindUniformVec4(_pPaint, "v_color_target", Vec4(c.r, c.g, c.b, c.a));
}

void SmearRGBA::setTargetColor(const cocos2d::Color4B &c)
{
    this->setTargetColor(Color4F(c));
}

void SmearRGBA::setReverse(bool reverse)
{
    _bReverse = reverse;
    GLUtility::bindUniformInt(_pPaint, "b_reverse_shape", _bReverse ? 1 : 0);
}

bool SmearRGBA::isReverse()
{
    return _bReverse;
}

void SmearRGBA::onEnter()
{
    SmearNode::onEnter();
}

void SmearRGBA::onExit()
{
    SmearNode::onExit();
}
