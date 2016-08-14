//
//  Scribble.cpp
//  GIRLS_Framework_part
//
//  Created by renhong on 9/12/13.
//
//

#include "Scribble.h"
#include "ScribbleUtil.h"
#define STRINGIFY(A)  #A
#include "ScribbleShader.frag"

Scribble::Scribble(Sprite *pBrush) : _brush(pBrush), _brushType(Scribble::eBrush), _target(nullptr){
    _brush->retain();
    _antiAliasingBrush = false;
}

Scribble::~Scribble(){
    _brush->release();
    CC_SAFE_RELEASE_NULL(_target);
}

void Scribble::antiAliasing(RenderTexture *pRenderTexture){
    pRenderTexture->getSprite()->getTexture()->setAntiAliasTexParameters();
}

void Scribble::disAntiAliasing(RenderTexture *pRenderTexture){
    pRenderTexture->getSprite()->getTexture()->setAliasTexParameters();
}

void Scribble::setAntiAliasingBrush(bool pAntiAliasingBrush){
    _antiAliasingBrush = pAntiAliasingBrush;
    int lAntiAliasingBrush = pAntiAliasingBrush ? 1 : 0;
    GLProgramState *lState = GLProgramState::getOrCreateWithGLProgram(this->getBrush()->getGLProgram());
    lState->setUniformInt("anti_aliasing_brush", lAntiAliasingBrush);
}

void Scribble::setBrush(Texture2D *pBrushTexture){
    SpriteFrame *lFrame = SpriteFrame::createWithTexture(pBrushTexture, Rect(0, 0, pBrushTexture->getContentSize().width, pBrushTexture->getContentSize().height));
    _brush->setDisplayFrame(lFrame);
    this->setBrushTexture();
}

void Scribble::setBrushShader(){
    string lGLProgramKey = ScribbleUtil::getInstance()->createUniqueGLProgramName(kScribbleShaderName, this);
    GLProgram *lProgram = ScribbleUtil::getInstance()->loadGLProgram(ccPositionTextureColor_noMVP_vert,ScribbleShader_frag, lGLProgramKey);
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

void Scribble::setBrushType(Scribble::BrushType pType){
    _brushType = pType;
    Sprite *lBrush = this->getBrush();
    switch (pType) {
        case Scribble::eBrush:{
            BlendFunc f = _antiAliasingBrush ? BlendFunc({GL_ONE, GL_ONE_MINUS_SRC_ALPHA }) : BlendFunc({GL_ONE, GL_ZERO});
            lBrush->setBlendFunc(f);
            break;
        }
        case eAdditiveBrush:{
            BlendFunc f = {GL_ONE, GL_ONE_MINUS_SRC_ALPHA};
            lBrush->setBlendFunc(f);
            break;
        }
        case Scribble::eAdditiveBrush_Src:{
            BlendFunc f = {GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA};//{GL_ONE, GL_ZERO};//
            lBrush->setBlendFunc(f);
            
            break;
        }
        case Scribble::eEaser:{
            BlendFunc f = {GL_ZERO,GL_SRC_ALPHA };
            lBrush->setBlendFunc(f);
            break;
        }
        default:
            break;
    }
}

void Scribble::setBrushAlphaTestValue(float pAlphaValue){
    CCAssert(pAlphaValue <= 1.0f && pAlphaValue >= 0.0f, "Apha value must in [0.0,1.0]");
    GLProgramState *lState = GLProgramState::getOrCreateWithGLProgram(this->getBrush()->getGLProgram());
    lState->setUniformFloat("f_alpha_value_brush", pAlphaValue);
}

void Scribble::setBrushTexture(){
    //CCLOG("Warning: Dot use this function frequently");
    Sprite *lBrush = this->getBrush();
    GLProgramState *lState = GLProgramState::getOrCreateWithGLProgram(this->getBrush()->getGLProgram());
    lState->setUniformVec2("v_texSize_brush", Vec2(lBrush->getContentSize().width, lBrush->getContentSize().height));
    lState->setUniformTexture("s_texture_brush", lBrush->getTexture());
    //lState->applyUniforms();////rh????
}

void Scribble::setTarget(Sprite *pTarget){
    //CCLOG("Warning: Dot use this function frequently");
    if (nullptr != _target && pTarget->getTexture() == _target->getTexture()) {
        return;
    }
    if (nullptr == _target) {
        _target = pTarget;
        _target->retain();
    }
    else{
        pTarget->retain();
        _target->release();
        _target = pTarget;
    }
    
    this->setTargetTexture(pTarget);
}

void Scribble::setTargetSolid(bool pIsSolid, float pSolidRate){
    int lIsSolid = pIsSolid ? 1 : 0;
    GLProgramState *lState = GLProgramState::getOrCreateWithGLProgram(this->getBrush()->getGLProgram());
    lState->setUniformInt("v_solid_brush", lIsSolid);
    lState->setUniformFloat("v_solid_rate", pSolidRate);
}

void Scribble::setTargetAlphaTestValue(float pAlphaValue){
    CCAssert(pAlphaValue <= 1.0f && pAlphaValue >= 0.0f, "Apha value must in [0.0,1.0]");
    GLProgramState *lState = GLProgramState::getOrCreateWithGLProgram(this->getBrush()->getGLProgram());
    lState->setUniformFloat("f_alpha_value_target", pAlphaValue);
}

void Scribble::setTargetTexture(Sprite *pTarget){
    if (nullptr == pTarget) {
        return;
    }
    
    GLProgramState *lState = GLProgramState::getOrCreateWithGLProgram(this->getBrush()->getGLProgram());
    lState->setUniformVec2("v_texSize_target", Vec2(pTarget->getContentSize().width, pTarget->getContentSize().height));
    lState->setUniformTexture("s_texture_target", pTarget->getTexture());
}

void Scribble::setPaintPosition(Sprite *pTarget, Sprite *pBrush, Point pPaintPosition){
    Sprite *lBrush = this->getBrush();
    Sprite *lTarget = this->getTarget();
    float lX = (pPaintPosition.x - lTarget->getPosition().x + lTarget->getContentSize().width / 2.0 - lBrush->getContentSize().width / 2.0);
    float lY = lTarget->getContentSize().height - (pPaintPosition.y - lTarget->getPosition().y + lTarget->getContentSize().height / 2.0 + lBrush->getContentSize().height / 2.0);
    GLProgramState *lState = GLProgramState::getOrCreateWithGLProgram(this->getBrush()->getGLProgram());
    lState->setUniformVec2("v_texCoord_target", Vec2(lX,lY));
}

void Scribble::paint(Sprite *pTarget, RenderTexture *pCanvas, Point pPosition){
    vector<Point> lPositions;
    lPositions.push_back(pPosition);
    this->paint(pTarget, pCanvas, lPositions);
}

void Scribble::paint(Sprite *pTarget, RenderTexture *pCanvas, Point pStartPosition, Point pEndPosition){
    Sprite *lBrush = this->getBrush();
    int lDelta = lBrush->getContentSize().width / 4.0 * lBrush->getScaleX();//5.0 * pBrush->getScale();  5.0 * pBrush->getScale();//
    vector<Point> lPositions = ScribbleUtil::getInstance()->interpolate(pStartPosition, pEndPosition, lDelta);
    this->paint(pTarget, pCanvas, lPositions);
}

void Scribble::paint(Sprite *pTarget, RenderTexture *pCanvas, vector<Point> pWorldPositions){
    Sprite *lBrush = this->getBrush();
    
    for (int i = 0; i < pWorldPositions.size(); ++i) {
        Point lPosition = pCanvas->getSprite()->convertToNodeSpace(pWorldPositions.at(i));
        Size lCanvasSize = pCanvas->getSprite()->getContentSize();
        Rect lCanvasBoundingBox = Rect(0, 0, lCanvasSize.width, lCanvasSize.height);
        if (lCanvasBoundingBox.containsPoint(lPosition)) {
            //set the coord of pen at the target
            this->setPaintPosition(pTarget, lBrush, lPosition);
            //draw
            lBrush->setPosition(lPosition);
            
            pCanvas->begin();
            lBrush->visit();
            pCanvas->end();
            Director::getInstance()->getRenderer()->render();
        }
    }
}

void Scribble::removeTargetObserver(RenderTexture *pTarget){
    NotificationCenter::getInstance()->removeObserver(pTarget, EVENT_COME_TO_BACKGROUND);
    NotificationCenter::getInstance()->removeObserver(pTarget, EVENT_COME_TO_FOREGROUND);
}
