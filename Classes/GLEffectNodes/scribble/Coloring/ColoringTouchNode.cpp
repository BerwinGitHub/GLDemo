//
//  ColoringTouchNode.cpp
//  DrawDemo
//
//  Created by renhong on 9/15/14.
//
//

#include "ColoringTouchNode.h"

ColoringTouchNode *ColoringTouchNode::create(string pTemplateFile, string pLineArtFile){
    ColoringTouchNode *ret = new ColoringTouchNode();
    if(ret && ret->init(pTemplateFile, pLineArtFile)){
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool ColoringTouchNode::init(string pTemplateFile, string pLineArtFile){
    if (!ColoringNode::init(pTemplateFile, pLineArtFile)) {
        return false;
    }
    //响应点击事件：根据点击位置，够造相应着色参数给着色器。
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = [this](cocos2d::Touch *pTouch, cocos2d::Event *pEvent)->bool{
        return this->coloring(pTouch->getLocation());
    };
    listener->onTouchMoved = [](cocos2d::Touch *pTouch, cocos2d::Event *pEvent)->void{};
    listener->onTouchEnded = [](cocos2d::Touch *pTouch, cocos2d::Event *pEvent)->void{};
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
    
    return true;
}