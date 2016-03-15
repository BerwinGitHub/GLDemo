//
//  GLHSVScene.cpp
//  GLDemo
//
//  Created by tangbowen on 11/3/16.
//
//

#include "GLHSLScene.hpp"

GLHSLScene::GLHSLScene():
_pHSLSprite(nullptr)
{
}

Scene* GLHSLScene::scene()
{
    auto scene = Scene::create();
    auto layer = GLHSLScene::create();
    scene->addChild(layer);
    return scene;
}

bool GLHSLScene::init()
{
    if (!TestBaseScene::init()) {
        return false;
    }
    Size visibleSize = Director::getInstance()->getVisibleSize();
    _pHSLSprite = HSLSprite::createWithHSL("images/dim.png", HSLSprite::HSL{0, 0, 0});
    _pHSLSprite->setPosition(visibleSize * 0.5f);
    this->addChild(_pHSLSprite);
    
    ControlSlider *slider = ControlSlider::create("images/slider_bg.png", "images/slider_progress.png", "images/slider_thumb.png");
    slider->setPosition(Vec2(visibleSize.width * 0.5f, visibleSize.height - 150));
    this->addChild(slider);
    slider->setValue(_pHSLSprite->getHSL().h);
    slider->setMinimumValue(-180);
    slider->setMaximumValue(180);
    slider->addTargetWithActionForControlEvents(this,cccontrol_selector(GLHSLScene::sliderChanged),Control::EventType::VALUE_CHANGED);
    
    ControlSlider *sSlider = ControlSlider::create("images/slider_bg.png", "images/slider_progress.png", "images/slider_thumb.png");
    sSlider->setPosition(Vec2(visibleSize.width * 0.5f, visibleSize.height - 250));
    this->addChild(sSlider);
    sSlider->setValue(_pHSLSprite->getHSL().h);
    sSlider->setMinimumValue(-1.0f);
    sSlider->setMaximumValue(1.0f);
    sSlider->addTargetWithActionForControlEvents(this,cccontrol_selector(GLHSLScene::sSliderChanged),Control::EventType::VALUE_CHANGED);
    
    ControlSlider *lSlider = ControlSlider::create("images/slider_bg.png", "images/slider_progress.png", "images/slider_thumb.png");
    lSlider->setPosition(Vec2(visibleSize.width * 0.5f, visibleSize.height - 350));
    this->addChild(lSlider);
    lSlider->setValue(_pHSLSprite->getHSL().h);
    lSlider->setMinimumValue(-1.0f);
    lSlider->setMaximumValue(1.0f);
    lSlider->addTargetWithActionForControlEvents(this,cccontrol_selector(GLHSLScene::lSliderChanged),Control::EventType::VALUE_CHANGED);
    return true;
}

void GLHSLScene::sliderChanged(cocos2d::Ref *ref, Control::EventType type)
{
    if (type == Control::EventType::VALUE_CHANGED) {
        ControlSlider *slider = dynamic_cast<ControlSlider*>(ref);
        float v = slider->getValue();
        //        log("======================>%f", v);
        _pHSLSprite->setHue(v);
    }
}

void GLHSLScene::sSliderChanged(cocos2d::Ref *ref, Control::EventType type)
{
    if (type == Control::EventType::VALUE_CHANGED) {
        ControlSlider *slider = dynamic_cast<ControlSlider*>(ref);
        float v = slider->getValue();
        //        log("======================>%f", v);
        _pHSLSprite->setStauration(v);
    }
}

void GLHSLScene::lSliderChanged(cocos2d::Ref *ref, Control::EventType type)
{
    if (type == Control::EventType::VALUE_CHANGED) {
        ControlSlider *slider = dynamic_cast<ControlSlider*>(ref);
        float v = slider->getValue();
        //        log("======================>%f", v);
        _pHSLSprite->setLightness(v);
    }
}