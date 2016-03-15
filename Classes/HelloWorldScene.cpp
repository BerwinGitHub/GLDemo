#include "HelloWorldScene.h"
#include "DragSprite.hpp"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    LayerColor *layer = LayerColor::create(Color4B(0, 0, 0, 255));
    layer->setPosition(Vec2::ZERO);
    layer->setAnchorPoint(Vec2::ZERO);
    this->addChild(layer);
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    _pRippleSpr = RippleSprite::createWithGSLength("images/frozen_small.png", 8);
    _pRippleSpr->setPosition(visibleSize * 0.5f);
    this->addChild(_pRippleSpr);
    
    return true;
}

void HelloWorld::testHSLSprite()
{
    Size visibleSize = Director::getInstance()->getVisibleSize();
    _pHSLSprite = HSLSprite::createWithHSL("images/yifu.png", HSLSprite::HSL{0, 0, 0});
    _pHSLSprite->setPosition(visibleSize * 0.5f);
    this->addChild(_pHSLSprite);
    
    ControlSlider *slider = ControlSlider::create("images/slider_bg.png", "images/slider_progress.png", "images/slider_thumb.png");
    slider->setPosition(Vec2(visibleSize.width * 0.5f, visibleSize.height - 150));
    this->addChild(slider);
    slider->setValue(_pHSLSprite->getHSL().h);
    slider->setMinimumValue(-180);
    slider->setMaximumValue(180);
    slider->addTargetWithActionForControlEvents(this,cccontrol_selector(HelloWorld::sliderChanged),Control::EventType::VALUE_CHANGED);
}

void HelloWorld::sliderChanged(cocos2d::Ref *ref, Control::EventType type)
{
    if (type == Control::EventType::VALUE_CHANGED) {
        ControlSlider *slider = dynamic_cast<ControlSlider*>(ref);
        float v = slider->getValue();
//        log("======================>%f", v);
        _pHSLSprite->setHue(v);
    }
    
}


void HelloWorld::menuCloseCallback(Ref* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
    return;
#endif

    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
