//
//  ScribbleTouchNode.cpp
//  ScribbleDemo_3_2
//
//  Created by renhong on 9/12/14.
//
//

#include "ScribbleTouchNode.h"

ScribbleTouchNode *ScribbleTouchNode::create(Size pCanvasSize){
    ScribbleTouchNode *ret = new ScribbleTouchNode();
    if(ret && ret->init(pCanvasSize)){
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

ScribbleTouchNode::ScribbleTouchNode(){
    _enableTouchBegan = true;
    _enableTouchMoved = true;
    _enableTouchEnded = true;
}

ScribbleTouchNode::~ScribbleTouchNode(){
    this->getEventDispatcher()->removeEventListener(_touchListener);
    _touchListener->release();
}

bool ScribbleTouchNode::init(Size pCanvasSize){
    if ( !ScribbleNode::init(pCanvasSize) ){
        return false;
    }
    
    _touchListener = EventListenerTouchOneByOne::create();
    _touchListener->retain();
    _touchListener->setSwallowTouches(true);
    _touchListener->onTouchBegan = CC_CALLBACK_2(ScribbleTouchNode::touchBegan, this);
    _touchListener->onTouchMoved = CC_CALLBACK_2(ScribbleTouchNode::touchMoved, this);
    _touchListener->onTouchEnded = CC_CALLBACK_2(ScribbleTouchNode::touchEnded, this);
    
    this->getEventDispatcher()->addEventListenerWithFixedPriority(_touchListener, 1);
    return true;
}

bool ScribbleTouchNode::touchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent){
    if (this->isEnabled()) {
        if (_enableTouchBegan) {
            Point lTouchPoint = pTouch->getLocation();
            this->paint(lTouchPoint);
        }
        return true;
    }
    
    return false;
}

void ScribbleTouchNode::touchMoved(cocos2d::Touch *pTouch, cocos2d::Event *pEvent){
    if (_enableTouchMoved) {
        Point lTouchPoint = pTouch->getLocation();
        Point lPreTouchPoint = pTouch->getPreviousLocation();
        this->paint(lTouchPoint, lPreTouchPoint);
    }
}

void ScribbleTouchNode::touchEnded(cocos2d::Touch *pTouch, cocos2d::Event *pEvent){
    if (_enableTouchEnded) {
        Point lTouchPoint = pTouch->getLocation();
        this->paint(lTouchPoint);
    }
}

