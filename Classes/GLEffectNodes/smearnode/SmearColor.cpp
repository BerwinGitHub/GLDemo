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
{
    
}

SmearColor::~SmearColor()
{
    
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

void SmearColor::onEnter()
{
    SmearNode::onEnter();
}

void SmearColor::onExit()
{
    SmearNode::onExit();
}