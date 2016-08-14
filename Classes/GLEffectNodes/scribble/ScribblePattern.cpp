//
//  ScribblePattern.cpp
//  ScribbleDemo_3_2
//
//  Created by renhong on 9/26/14.
//
//

#include "ScribblePattern.h"
#include "ScribbleUtil.h"
#define STRINGIFY(A)  #A
#include "ScribblePatternShader.frag"

ScribblePattern::ScribblePattern(Sprite *pBrush) : Scribble(pBrush){
    _pattern = nullptr;
}

ScribblePattern::~ScribblePattern(){
    CC_SAFE_RELEASE(_pattern);
}

void ScribblePattern::setBrushShader(){
    string lGLProgramKey = ScribbleUtil::getInstance()->createUniqueGLProgramName(kScribbleShaderName, this);
    GLProgram *lProgram = ScribbleUtil::getInstance()->loadGLProgram(ccPositionTextureColor_noMVP_vert,ScribblePatternShader_frag, lGLProgramKey);
    if (NULL != lProgram) {
        if (this->getBrush()->getGLProgram() != lProgram) {
            ScribbleUtil::getInstance()->setGLProgram(this->getBrush(), lProgram);
            
            this->setBrushAlphaTestValue(0.0);
            this->setTargetAlphaTestValue(0.0);
            this->setTargetSolid(true);
            this->setBrushTexture();
        }
    }
}

//void ScribblePattern::setBrushType(Scribble::BrushType pType){
//    Scribble::setBrushType(pType);
//    Sprite *lBrush = this->getBrush();
//    switch (pType) {
//        case Scribble::eBrush:{
//            BlendFunc f = {GL_ONE, GL_ONE_MINUS_SRC_ALPHA };
//            lBrush->setBlendFunc(f);
//            break;
//        }
//        default:
//            break;
//    }
//}

void ScribblePattern::setPattern(Sprite *pPartten){
    //CCLOG("Warning: Dot use this function frequently");
    if (nullptr != _pattern && pPartten->getTexture() == _pattern->getTexture()) {
        return;
    }
    if (nullptr == _pattern) {
        _pattern = pPartten;
        _pattern->retain();
    }
    else{
        pPartten->retain();
        _pattern->release();
        _pattern = pPartten;
    }
    
    this->setPatternTexture(pPartten);
}

void ScribblePattern::setPatternTexture(Sprite *pPattern){
    if (nullptr == pPattern) {
        return;
    }
    Texture2D::TexParams tp;
    tp.magFilter = GL_NEAREST;
    tp.minFilter = GL_NEAREST;
    tp.wrapS = GL_CLAMP_TO_EDGE;
    tp.wrapT = GL_CLAMP_TO_EDGE;
    pPattern->getTexture()->setTexParameters(tp);
    
    GLProgramState *lState = GLProgramState::getOrCreateWithGLProgram(this->getBrush()->getGLProgram());
    lState->setUniformVec2("v_texSize_pattern", Vec2(pPattern->getContentSize().width, pPattern->getContentSize().height));
    lState->setUniformTexture("s_texture_pattern", pPattern->getTexture());
}

void ScribblePattern::setTolerance(int pTolerance/* = 0*/){
    GLProgramState *lState = GLProgramState::getOrCreateWithGLProgram(this->getBrush()->getGLProgram());
    lState->setUniformFloat("f_tolerance", pTolerance / 255.0);
}

void ScribblePattern::setTarget(Sprite *pTarget){
    Scribble::setTarget(pTarget);
    GLProgramState *lState = GLProgramState::getOrCreateWithGLProgram(this->getBrush()->getGLProgram());
    lState->setUniformInt("v_taret_type_color", 0);
}

void ScribblePattern::setTarget(Color4F pColor){
    GLProgramState *lState = GLProgramState::getOrCreateWithGLProgram(this->getBrush()->getGLProgram());
    lState->setUniformVec4("v_color_target", Vec4(pColor.r, pColor.g, pColor.b, pColor.a));
    lState->setUniformInt("v_taret_type_color", 1);
}

void ScribblePattern::setPatternColor(Color4F pColor){
    GLProgramState *lState = GLProgramState::getOrCreateWithGLProgram(this->getBrush()->getGLProgram());
    lState->setUniformVec4("v_pattern_color", Vec4(pColor.r, pColor.g, pColor.b, pColor.a));
}