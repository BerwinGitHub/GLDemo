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
#include "SmearSegment.hpp"
#include "ScribblePatternNode.h"
#include "ScribbleUtil.h"

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
    
//    Sprite *top = Sprite::create("images/segment_top.png");
//    top->setPosition(visibleSize * 0.5f);
//    this->addChild(top, 1);
//    
//    ScribblePatternNode *spn = ScribblePatternNode::create(top->getContentSize());
//    spn->setPosition(visibleSize * 0.5f);
//    this->addChild(spn);
//    spn->useBrush("images/solid_32_feather.png");
//    spn->setPattern("images/segment.png");
////    spn->setTolerance(110);
////    spn->useTarget(Color4F(1.0f, 0, 0, 1.0f));
//    spn->useTarget(Sprite::create("images/segment_target_big.png"));
//    
//    Image *img = new Image();
//    img->initWithImageFile("images/segment.png");
//    
//    EventListenerTouchOneByOne *e1 = EventListenerTouchOneByOne::create();
//    e1->setSwallowTouches(true);
//    e1->onTouchBegan = [=](Touch* t, Event *e){
//        Vec2 pos = top->convertToNodeSpace(t->getLocation());
//        Color4B c4b = ScribbleUtil::getInstance()->getColor(img, pos);
//        Color4F c = Color4F(c4b);
//        log("r:%.2f - g:%.2f - b:%.2f - a:%.2f", c.r, c.g, c.b, c.a);
//        spn->setPatternColor(c);
//        return true;
//    };
//    e1->onTouchMoved = [=](Touch* t, Event *e){
//        spn->paint(t->getPreviousLocation(), t->getLocation());
//    };
//    e1->onTouchEnded = [=](Touch* t, Event *e){
//    };
//    _eventDispatcher->addEventListenerWithSceneGraphPriority(e1, this);
    
    this->testSmearSegment(visibleSize);
//    this->testSmearRGBA(visibleSize);
//    this->testSmearArmature(visibleSize);
//    this->testSmearNode(visibleSize);
    
    return true;
}

void SmearScene::testSmearSegment(const Size &s)
{
    
    SmearSegment *ss = SmearSegment::createWithSegment("images/segment.png", "images/segment_target.png");
    ss->setPaint("images/solid_32_feather.png");
    ss->setPaintType(SmearNode::PaintType::kPaint);
    ss->setPosition(s * 0.5f);
    ss->SmearNode::setTarget("images/segment_target_big.png");
    this->addChild(ss);
    
    Sprite *top = Sprite::create("images/segment_top.png");
    top->setPosition(s * 0.5f);
    this->addChild(top);
    
    
    EventListenerTouchOneByOne *e1 = EventListenerTouchOneByOne::create();
    e1->setSwallowTouches(true);
    e1->onTouchBegan = [=](Touch* t, Event *e){
        Color4F c = ss->getPixelColor(t->getLocation());
        log("r:%.2f - g:%.2f - b:%.2f - a:%.2f", c.r, c.g, c.b, c.a);
        ss->setSegmentColor(c);
//        ss->setTargetColor(Color4B(rand() % 255, rand() % 255, rand() % 255, 255));
        return true;
    };
    e1->onTouchMoved = [=](Touch* t, Event *e){
        ss->draw(t->getPreviousLocation(), t->getLocation());
    };
    e1->onTouchEnded = [=](Touch* t, Event *e){
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(e1, this);
}

void SmearScene::testSmearRGBA(const cocos2d::Size &s)
{
    SmearRGBA *srgba = SmearRGBA::createWithRGBA("animation/cake/cream/1.png", Color4B(255, 0, 0, 255));
    srgba->setPaint("images/solid_32_feather.png");
    srgba->setPaintType(SmearNode::PaintType::kPaint);
    srgba->setPosition(s * 0.5f);
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
}

void SmearScene::testSmearArmature(const cocos2d::Size &vs)
{
    
    ArmatureDataManager::getInstance()->addArmatureFileInfo("animation/cake/cake.ExportJson");
    
    Armature *cake = Armature::create("cake");
    cake->setPosition(vs * 0.5f);
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
}

void SmearScene::testSmearNode(const cocos2d::Size &s)
{
    
}
