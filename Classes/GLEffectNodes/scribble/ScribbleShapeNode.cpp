//
//  ScribbleShapeNode.cpp
//  ScribbleDemo_3_2
//
//  Created by renhong on 9/26/14.
//
//

#include "ScribbleShapeNode.h"

ScribbleShapeNode *ScribbleShapeNode::create(Size pCanvasSize){
    ScribbleShapeNode *ret = new ScribbleShapeNode();
    if(ret && ret->init(pCanvasSize))
    {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

ScribbleShapeNode::ScribbleShapeNode(){
    CC_SAFE_DELETE(_scribble);////rh 重构，将初始化放入init方法
    _scribble = new ScribbleShape(Brush::create());
    _scribble->setBrushShader();
    _scribble->setBrushType(Scribble::eBrush);
    _scribble->setTargetSolid(true);
}

bool ScribbleShapeNode::init(Size pCanvasSize){
    if (!ScribbleNode::init(pCanvasSize)) {
        return false;
    }

//    BlendFunc f = {GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA};
    this->getCanvas()->getSprite()->setBlendFunc(BlendFunc::ALPHA_PREMULTIPLIED);
    
    return true;
}

bool ScribbleShapeNode::setShape(string pShapeFile, Point pPosition){
    Sprite *lSprite = Sprite::create(pShapeFile);
    lSprite->setPosition(pPosition);
    this->getScribble()->setShape(lSprite);
    return true;
}

bool ScribbleShapeNode::setShape(string pShapeFile){
    return this->setShape(pShapeFile, this->getCanvasSize() / 2);
}

bool ScribbleShapeNode::useTargetColor(Color4F pTargetColor){
    this->getScribble()->setTarget(pTargetColor);
    return true;
}