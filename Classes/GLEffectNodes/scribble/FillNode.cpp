//
//  FillNode.cpp
//  ScribbleDemo_3_2
//
//  Created by renhong on 15/12/16.
//
//

#include "FillNode.h"

FillNode *FillNode::create(Size pCanvasSize){
    FillNode *ret = new FillNode();
    if(ret && ret->init(pCanvasSize)){
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

FillNode::FillNode(){
    _coloringNode = nullptr;
}

bool FillNode::init(Size pCanvasSize){
    if (!ScribbleLineFrameNode::init(pCanvasSize)) {
        return false;
    }
    this->_coloringNode = ColoringLineFrameNode::createWithCanvas(this->_canvas);
    this->addChild(this->_coloringNode);
//    this->_coloringNode->debugPattern(false);
    this->_coloringNode->setPosition(Vec2(-this->_coloringNode->getContentSize().width / 2, -this->_coloringNode->getContentSize().height / 2));
    return true;
}

bool FillNode::setPattern(string pLineFrameFile, bool pAutoFill/* = false*/, int pTolerance/* = 0*/){
    bool lRes = ScribbleLineFrameNode::setPattern(pLineFrameFile, pAutoFill, pTolerance);
    if(lRes){
        this->getColoringNode()->setPattern(this->getPatternNode());
        this->getColoringNode()->uploadRenderPara();
        
        this->getColoringNode()->setStartColor(Color4B(255, 0, 0, 255));
        this->getColoringNode()->setColoringMode(ColoringNode::ColoringMode::eFlat);
    }
    
    
    return lRes;
}

void FillNode::loadEnded(Pattern *pNode){
    ScribbleLineFrameNode::loadEnded(pNode);
    this->getColoringNode()->updatePattern();
}