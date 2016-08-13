//
//  SmearScene.cpp
//  GLDemo
//
//  Created by tangbowen on 11/3/16.
//
//

#include "SmearScene.hpp"
#include "SmearNode.hpp"
#include "SmearArmature.hpp"
#include "cocostudio/CocoStudio.h"
//#include "SmearColor.hpp"
#include "SmearRGBA.hpp"

using namespace cocostudio;

SmearScene::SmearScene()
{
}

SmearScene::~SmearScene()
{
    this->removeAllChildren();
    Director::getInstance()->getTextureCache()->removeUnusedTextures();
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
//    SmearColor *sc = SmearColor::createWithShape(Shape::create("images/scrrible_rect.png"));
//    sc->setPaintTexture(Paint::create("images/solid_32_feather.png")->getTexture());
//    sc->setPaintType(SmearNode::PaintType::kPaint);
//    sc->setTargetColor(Color4B(255, 0, 0, 255));
//    sc->setPosition(visibleSize * 0.5f);
//    this->addChild(sc);
    SmearRGBA *srgba = SmearRGBA::createWithRGBA("animation/cake/cream/1.png", Color4B(255, 0, 0, 255));
    srgba->setPaint("images/solid_32_feather.png");
    srgba->setPaintType(SmearNode::PaintType::kPaint);
    srgba->setPosition(visibleSize * 0.5f);
    this->addChild(srgba);
//    srgba->setAntiAliasingPaint(false);
    srgba->antiAliasing();
//    srgba->SmearNode::setTarget("animation/cake/cream/1.png");
//    srgba->SmearNode::setTarget("images/dim.png");
    
    EventListenerTouchOneByOne *e1 = EventListenerTouchOneByOne::create();
    e1->setSwallowTouches(true);
    e1->onTouchBegan = [=](Touch* t, Event *e){
        return true;
    };
    e1->onTouchMoved = [=](Touch* t, Event *e){
        srgba->draw(t->getPreviousLocation(), t->getLocation());
    };
    e1->onTouchEnded = [=](Touch* t, Event *e){
        srgba->clearSelf();
        srgba->setReverse(!srgba->isReverse());
//        static int i = 1;
//        i = ++i % 16 + 1;
//        srgba->SmearNode::setTarget(StringUtils::format("animation/cake/cream/%d.png", i));
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(e1, this);
    
    return true;
    
    ArmatureDataManager::getInstance()->addArmatureFileInfo("animation/cake/cake.ExportJson");
    
    Armature *cake = Armature::create("cake");
    cake->setPosition(visibleSize * 0.5f);
    this->addChild(cake);
    
    SmearArmature *s = SmearArmature::createWithBone(cake->getBone("cream_1"), "animation/cake/cream/1.png", "images/solid_32_feather.png");
//    s->setPaintType(SmearNode::PaintType::kEaser);
    s->setPaintHardness(0.2f);
    
//    SmearNode *s = SmearNode::create(Sprite::create("images/dim.png"), Sprite::create("images/solid_32_feather.png"));
//    s->setPosition(visibleSize * 0.5f);
//    s->setPaintHardness(0.5);
//    this->addChild(s);
    
    EventListenerTouchOneByOne *e = EventListenerTouchOneByOne::create();
    e->setSwallowTouches(true);
    e->onTouchBegan = [=](Touch* t, Event *e){
        return true;
    };
    e->onTouchMoved = [=](Touch* t, Event *e){
        s->draw(t->getPreviousLocation(), t->getLocation());
    };
    e->onTouchEnded = [=](Touch* t, Event *e){
//        s->setPaintType(s->getPaintType() == SmearNode::PaintType::kEaser ? SmearNode::PaintType::kPaint : SmearNode::PaintType::kEaser);
//        s->setPaintHardness(0.1f);
        static int i = 1;
        i = ++i % 16 + 1;
        s->setTarget(Target::create(StringUtils::format("animation/cake/cream/%d.png", i))->getTexture());
//        s->drawSelf();
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(e, this);
//    auto seq = Sequence::create(MoveBy::create(2.0f, Vec2(0, -200)), MoveBy::create(2.0f, Vec2(0, 200)), nullptr);
//    s->runAction(RepeatForever::create(seq));
    
    return true;
}
