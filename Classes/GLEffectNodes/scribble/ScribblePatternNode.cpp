//
//  ScribblePatternNode.cpp
//  ScribbleDemo_3_2
//
//  Created by renhong on 9/26/14.
//
//

#include "ScribblePatternNode.h"

ScribblePatternNode *ScribblePatternNode::create(Size pCanvasSize){
    ScribblePatternNode *ret = new ScribblePatternNode();
    if(ret && ret->init(pCanvasSize))
    {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

ScribblePatternNode::ScribblePatternNode(){
    _patternImage = nullptr;
    CC_SAFE_DELETE(_scribble);////rh 重构，将初始化放入init方法
    _scribble = new ScribblePattern(Brush::create());
    _scribble->setBrushShader();
    _scribble->setBrushType(Scribble::eBrush);
    _scribble->setTargetSolid(true);
}

ScribblePatternNode::~ScribblePatternNode(){
    CC_SAFE_RELEASE(_patternImage);
}

bool ScribblePatternNode::init(Size pCanvasSize){
    if (!ScribbleNode::init(pCanvasSize)) {
        return false;
    }

//    BlendFunc f = {GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA};
    this->getCanvas()->getSprite()->setBlendFunc(BlendFunc::ALPHA_PREMULTIPLIED);
    
    return true;
}

bool ScribblePatternNode::setPattern(string pShapeFile, Point pPosition){
    Sprite *lSprite = Sprite::create(pShapeFile);
    lSprite->getTexture()->setAliasTexParameters();
    lSprite->setPosition(pPosition);
    this->getScribble()->setPattern(lSprite);
    
    CC_SAFE_RELEASE(_patternImage);
    _patternImage = new Image();
    _patternImage->initWithImageFile(pShapeFile);
    return true;
}

bool ScribblePatternNode::setPattern(string pShapeFile){
    return this->setPattern(pShapeFile, this->getCanvasSize() / 2);
}

void ScribblePatternNode::setTolerance(int pTolerance/* = 0*/){
    this->getScribble()->setTolerance(pTolerance);
}

bool ScribblePatternNode::useTarget(Color4F pTargetColor){
    this->getScribble()->setTarget(pTargetColor);
    return true;
}

bool ScribblePatternNode::useTarget(Sprite *pTarget){
    this->getScribble()->setTarget(pTarget);
    return true;
}

bool ScribblePatternNode::setPatternColor(Color4F pColor){
    this->getScribble()->setPatternColor(pColor);
    return true;
}