//
//  ControllerScene.cpp
//  GLDemo
//
//  Created by tangbowen on 11/3/16.
//
//

#include "ControllerScene.hpp"
#include "TestBaseScene.hpp"
#include "GLHSLScene.hpp"
#include "GLRippleScene.hpp"
#include "BlurScene.hpp"
#include "SmearScene.hpp"

#define LIST_NUM            4
#define LINE_SPACE          100

static Vec2 s_tCurPos = Vec2::ZERO;

typedef struct _Controller{
    const char *test_name;
    function<Scene*()> callback;
} Controller;


Controller g_aTestNames[LIST_NUM] = {
    { "GL:HSL(HSV) Sprite",  [](){ return GLHSLScene::scene();} },
    { "GL:Ripple Sprite",    [](){return GLRippleScene::scene(); } },
    { "GL:Blur Sprite",     [](){return BlurScene::scene(); } },
    { "GL:Smear Sprite",    [](){return SmearScene::scene(); } }
};

ControllerScene::ControllerScene():
_itemMenu(nullptr),
_beginPos(Vec2::ZERO)
{
    
}

ControllerScene::~ControllerScene()
{
    this->removeAllChildren();
    Director::getInstance()->getTextureCache()->removeUnusedTextures();
}

Scene* ControllerScene::scene()
{
    auto scene = Scene::create();
    auto layer = ControllerScene::create();
    scene->addChild(layer);
    return scene;
}

bool ControllerScene::init()
{
    if(!Layer::init()){
        return false;
    }
    _visibleSize = Director::getInstance()->getVisibleSize();
    this->initCloseButton();
    this->initList();
    
    return true;
}

void ControllerScene::initList()
{
    
    // add menu items for tests
    TTFConfig ttfConfig("fonts/Marker Felt.ttf", 50);
    _itemMenu = Menu::create();
    for (int i = 0; i < LIST_NUM; ++i)
    {
        auto label = Label::createWithTTF(ttfConfig, g_aTestNames[i].test_name);
        auto menuItem = MenuItemLabel::create(label, CC_CALLBACK_1(ControllerScene::menuCallBack, this));
        
        _itemMenu->addChild(menuItem, i + 10000);
        menuItem->setPosition( Vec2( _visibleSize.width * 0.5f, (_visibleSize.height - (i + 1) * LINE_SPACE) ));
    }
    
    _itemMenu->setContentSize(Size(_visibleSize.width, (LIST_NUM + 1) * (LINE_SPACE)));
    _itemMenu->setPosition(s_tCurPos);
    addChild(_itemMenu);
    
    // Register Touch Event
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    
    listener->onTouchBegan = CC_CALLBACK_2(ControllerScene::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(ControllerScene::onTouchMoved, this);
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
    auto mouseListener = EventListenerMouse::create();
    mouseListener->onMouseScroll = CC_CALLBACK_1(ControllerScene::onMouseScroll, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(mouseListener, this);
}

void ControllerScene::menuCallBack(cocos2d::Ref *ref)
{ // get the userdata, it's the index of the menu item clicked
    auto menuItem = static_cast<MenuItem *>(ref);
    int idx = menuItem->getLocalZOrder() - 10000;
    
    // create the test scene and run it
    auto scene = g_aTestNames[idx].callback();
    if (scene) {
        Director::getInstance()->replaceScene(scene);
    }
}

bool ControllerScene::onTouchBegan(Touch* touch, Event  *event)
{
    _beginPos = touch->getLocation();
    return true;
}

void ControllerScene::onTouchMoved(Touch* touch, Event  *event)
{
    auto touchLocation = touch->getLocation();
    float nMoveY = touchLocation.y - _beginPos.y;
    
    auto curPos  = _itemMenu->getPosition();
    auto nextPos = Vec2(curPos.x, curPos.y + nMoveY);
    
    if (nextPos.y < 0.0f)
    {
        _itemMenu->setPosition(Vec2::ZERO);
        return;
    }
    
    if (nextPos.y > ((LIST_NUM + 1)* LINE_SPACE - _visibleSize.height))
    {
        _itemMenu->setPosition(Vec2(0, ((LIST_NUM + 1)* LINE_SPACE - _visibleSize.height)));
        return;
    }
    
    _itemMenu->setPosition(nextPos);
    _beginPos = touchLocation;
    s_tCurPos   = nextPos;
}

void ControllerScene::onMouseScroll(Event *event)
{
    auto mouseEvent = static_cast<EventMouse*>(event);
    float nMoveY = mouseEvent->getScrollY() * 6;
    
    auto curPos  = _itemMenu->getPosition();
    auto nextPos = Vec2(curPos.x, curPos.y + nMoveY);
    
    if (nextPos.y < 0.0f)
    {
        _itemMenu->setPosition(Vec2::ZERO);
        return;
    }
    
    if (nextPos.y > ((LIST_NUM + 1)* LINE_SPACE - _visibleSize.height))
    {
        _itemMenu->setPosition(Vec2(0, ((LIST_NUM + 1)* LINE_SPACE - _visibleSize.height)));
        return;
    }
    
    _itemMenu->setPosition(nextPos);
    s_tCurPos   = nextPos;
}

void ControllerScene::initCloseButton()
{
    //add the menu item for back to main menu
    TTFConfig ttfConfig("fonts/Marker Felt.ttf", 50);
    auto label = Label::createWithTTF(ttfConfig, "Exit");
    
    auto menuItem = MenuItemLabel::create(label, CC_CALLBACK_1(ControllerScene::closeMenuCallBack, this));
    auto menu = Menu::create(menuItem, nullptr);
    
    menu->setPosition( Vec2::ZERO );
    menuItem->setPosition( Vec2( _visibleSize.width - 35, 25));
    
    addChild(menu, UINT16_MAX - 1);
}

void ControllerScene::closeMenuCallBack(cocos2d::Ref *ref)
{
    Director::getInstance()->end();
}

void ControllerScene::onEnter()
{
    Layer::onEnter();
}

void ControllerScene::onExit()
{
    Layer::onExit();
}