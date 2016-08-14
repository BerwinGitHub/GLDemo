//
//  ScribbleWireframeNode.cpp
//  ScribbleDemo_3_2
//
//  Created by renhong on 4/2/15.
//
//

#include "ScribbleWireframeNode.h"
#include "ScribbleUtil.h"

ScribbleWireframeNode *ScribbleWireframeNode::create(Size pCanvasSize){
    ScribbleWireframeNode *ret = new ScribbleWireframeNode();
    if(ret && ret->init(pCanvasSize))
    {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

void ScribbleWireframeNode::setWireframe(Node *pWireframe, Vec2 pPosition){
    if (nullptr != _wireframe) {
        _wireframe->removeFromParent();
    }
    _wireframe = pWireframe;
    if (nullptr != pWireframe) {
        this->addChild(pWireframe);
        pWireframe->setPosition(pPosition);
    }
}

Color4B ScribbleWireframeNode::getPatternColor(Vec2 pWorldPosition){
    Vec2 lPointInPattern = _wireframe->convertToNodeSpace(pWorldPosition);
    
    Color4B lColor = ScribbleUtil::getInstance()->getColor(this->getPatternImage(), lPointInPattern);
    return lColor;
}

void ScribbleWireframeNode::setPatternColor(Vec2 pWorldPosition){
    Color4B lColor = this->getPatternColor(pWorldPosition);
    Color4F lTouchedColor(lColor);
    
    ScribblePatternNode::setPatternColor(lTouchedColor);
}