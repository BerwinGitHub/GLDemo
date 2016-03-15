//
//  TestBaseLayer.cpp
//  GLDemo
//
//  Created by tangbowen on 11/3/16.
//
//

#include "TestBaseScene.hpp"

TestBaseScene::TestBaseScene()
{
    
}

TestBaseScene::~TestBaseScene()
{
    this->removeAllChildren();
    Director::getInstance()->getTextureCache()->removeUnusedTextures();
}

bool TestBaseScene::init()
{
    if (!Layer::init()) {
        return false;
    }
    _visibleSize = Director::getInstance()->getVisibleSize();
    this->initTitle();
    this->initReturnMenu();
    return true;
}

void TestBaseScene::initTitle()
{
    TTFConfig ttfConfig("fonts/Marker Felt.ttf", 50);
    auto label = Label::createWithTTF(ttfConfig, this->title());
    label->setPosition(Vec2(_visibleSize.width * 0.5f, _visibleSize.height - 50));
    addChild(label, UINT16_MAX - 1);
    
}

void TestBaseScene::initReturnMenu()
{
    
    
    //add the menu item for back to main menu
    TTFConfig ttfConfig("fonts/Marker Felt.ttf", 35);
    auto label = Label::createWithTTF(ttfConfig, "MainMenu");
    
    auto menuItem = MenuItemLabel::create(label, CC_CALLBACK_1(TestBaseScene::menuCallBack, this));
    auto menu = Menu::create(menuItem, nullptr);
    
    menu->setPosition( Vec2::ZERO );
    menuItem->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
    menuItem->setPosition( Vec2( _visibleSize.width - 25, 25));
    
    addChild(menu, UINT16_MAX - 1);
}

void TestBaseScene::menuCallBack(Ref *sender )
{
    Director::getInstance()->replaceScene(ControllerScene::scene());
}

void TestBaseScene::onEnter()
{
    Layer::onEnter();
}

void TestBaseScene::onExit()
{
    Layer::onExit();
}