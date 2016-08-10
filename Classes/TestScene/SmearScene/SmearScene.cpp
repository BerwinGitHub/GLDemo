//
//  SmearScene.cpp
//  GLDemo
//
//  Created by tangbowen on 11/3/16.
//
//

#include "SmearScene.hpp"
#include "SmearNode.hpp"
#include "ScribbleNode.h"

SmearScene::SmearScene()
{
}

Scene* SmearScene::scene()
{
    auto scene = Scene::create();
    auto layer = SmearScene::create();
    scene->addChild(layer);
    return scene;
}

bool SmearScene::init()
{
    if (!TestBaseScene::init()) {
        return false;
    }
    Size visibleSize = Director::getInstance()->getVisibleSize();
    
//    ScribbleNode *s = ScribbleNode::create(485, 420);
//    s->setPosition(visibleSize * 0.5f);
//    s->useTarget("images/dim.png", Vec2(485 / 2, 420 / 2));
//    s->useBrush("images/solid_brush_48.png");
//    s->useBrush(Scribble::BrushType::eBrush);
//    this->addChild(s);
    
    SmearNode *s = SmearNode::create(Sprite::create("images/dim.png"), Sprite::create("images/bath_brush.png"));
    s->setPosition(visibleSize * 0.5f);
    this->addChild(s);
    
    EventListenerTouchOneByOne *e = EventListenerTouchOneByOne::create();
    e->setSwallowTouches(true);
    e->onTouchBegan = [=](Touch* t, Event *e){
        return true;
    };
    e->onTouchMoved = [=](Touch* t, Event *e){
        s->draw(t->getPreviousLocation(), t->getLocation());
//        s->paint(t->getPreviousLocation(), t->getLocation());
    };
    e->onTouchEnded = [=](Touch* t, Event *e){
        s->setPaintType(s->getPaintType() == SmearNode::PaintType::kEaserFade ? SmearNode::PaintType::kPaint : SmearNode::PaintType::kEaserFade);
//        s->useBrush(Scribble::BrushType::eEaser);
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(e, this);
    auto seq = Sequence::create(MoveBy::create(2.0f, Vec2(0, -200)), MoveBy::create(2.0f, Vec2(0, 200)), nullptr);
    s->runAction(RepeatForever::create(seq));
    
    return true;
}