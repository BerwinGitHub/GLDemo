//
//  BlurScene.cpp
//  GLDemo
//
//  Created by tangbowen on 11/3/16.
//
//

#include "BlurScene.hpp"
#include "VisualEffectLayer.hpp"

BlurScene::BlurScene()
{
}

Scene* BlurScene::scene()
{
    auto scene = Scene::create();
    auto layer = BlurScene::create();
    scene->addChild(layer);
    return scene;
}

bool BlurScene::init()
{
    if (!TestBaseScene::init()) {
        return false;
    }
    Size visibleSize = Director::getInstance()->getVisibleSize();
    
    Sprite *bg = Sprite::create("images/ocean.png");
    bg->setPosition(visibleSize * 0.5f);
    this->addChild(bg);
    
    Sprite *dim = Sprite::create("images/dim.png");
    dim->setPosition((visibleSize - Size(0, 300)) * 0.5f);
    this->addChild(dim);
    dim->runAction(RepeatForever::create(Sequence::create(MoveBy::create(1.0f, Vec2(0, 150)), MoveBy::create(1.0f, Vec2(0, -150)), nullptr)));
    
    VisualEffectLayer *eff = VisualEffectLayer::createWithRadius(visibleSize * 0.5f, 100);
    eff->setPosition(visibleSize * 0.5f);
    this->addChild(eff);
    
    
    return true;
}