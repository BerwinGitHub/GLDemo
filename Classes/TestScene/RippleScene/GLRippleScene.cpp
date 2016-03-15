//
//  GLHSVScene.cpp
//  GLDemo
//
//  Created by tangbowen on 11/3/16.
//
//

#include "GLRippleScene.hpp"
#include "RippleSprite.hpp"

GLRippleScene::GLRippleScene()
{
}

Scene* GLRippleScene::scene()
{
    auto scene = Scene::create();
    auto layer = GLRippleScene::create();
    scene->addChild(layer);
    return scene;
}

bool GLRippleScene::init()
{
    if (!TestBaseScene::init()) {
        return false;
    }
    Size visibleSize = Director::getInstance()->getVisibleSize();
    
    RippleSprite *_pRippleSpr = RippleSprite::createWithGSLength("images/ocean.png", 8);
    _pRippleSpr->setPosition(visibleSize * 0.5f);
    this->addChild(_pRippleSpr);
    
    return true;
}