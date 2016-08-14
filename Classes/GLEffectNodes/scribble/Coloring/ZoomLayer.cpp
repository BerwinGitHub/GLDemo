//
//  ZoomLayer.cpp
//  ColourFillingTest
//
//  Created by renhong on 14-11-6.
//
//http://cn.cocos2d-x.org/tutorial/show?id=1479

#include "ZoomLayer.h"

ZoomLayer::~ZoomLayer(){
    this->getEventDispatcher()->removeEventListener(_touchEventListener);
}

ZoomLayer *ZoomLayer::create(Node *pNode){
    ZoomLayer *ret = new ZoomLayer();
    if(ret && ret->init(pNode)){
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool ZoomLayer::init(Node *pNode){
    if(!LayerColor::initWithColor(Color4B(0, 0, 0, 0))){
        return false;
    }
    mpOrigin = Point(0,0);
    _contentNode = pNode;
    this->setContentSize(pNode->getContentSize());
    pNode->setAnchorPoint(Point(0,0));
    this->addChild(pNode);
    this->ignoreAnchorPointForPosition(false);
    
    auto listener = EventListenerTouchAllAtOnce::create();
    _touchEventListener = listener;
    listener->setEnabled(true);
    //    this->_touchListener = listener;
    
    listener->onTouchesBegan = [this](const std::vector<Touch*>&pTouches, Event*pEvent)->void{
        _contentNode->setUserData((void *)0x1);
    };
    listener->onTouchesMoved = [this](const std::vector<Touch*>&touches, Event*pEvent)->void{
        _contentNode->setUserData((void *)0x0);
        Size winSize = _contentNode->getContentSize();//Size(768, 768);//Director::getInstance()->getWinSize();
        
        if(touches.size() > 1){// 多点进行缩放
            // 得到当前两触摸点
            auto point1 = _contentNode->getParent()->convertToNodeSpace(touches[0]->getLocation());
            auto point2 = _contentNode->getParent()->convertToNodeSpace(touches[1]->getLocation());
            // 计算两点之间得距离
            auto currDistance = touches[0]->getLocation().distance(touches[1]->getLocation());//point1.distance(point2);
            // 计算两触摸点上一时刻之间得距离
            auto prevDistance = touches[0]->getPreviousLocation().distance(touches[1]->getPreviousLocation());
            // 两触摸点与原点的差向量，pointVec1和pointVec2是相对于maplayer的位置
            auto pointVec1 = point1  - mpOrigin;
            auto pointVec2 = point2  - mpOrigin;
            // 两触摸点的相对中点
            auto relMidx = (pointVec1.x + pointVec2.x) / 2 ;
            auto relMidy = (pointVec1.y + pointVec2.y) / 2 ;
            // 计算maplayer的锚点
            auto anchorX = relMidx / _contentNode->getBoundingBox().size.width;
            auto anchorY = relMidy / _contentNode->getBoundingBox().size.height;
            // 相对屏幕的中点
            auto absMidx = (point2.x + point1.x) / 2 ;
            auto absMidy = (point2.y + point1.y) / 2 ;
            
            // 缩放时，为了避免出现空白的区域，需要做以下的边界处理。
            // 当bgSprite快要进入到屏幕时，修改maplayer的位置（既absMidx和absMidy）。
            if(  mpOrigin.x > 0){
                absMidx -= mpOrigin.x;
            }
            if( mpOrigin.x < -_contentNode->getBoundingBox().size.width + winSize.width ){
                absMidx +=  -_contentNode->getBoundingBox().size.width + winSize.width - mpOrigin.x;
            }
            if( mpOrigin.y > 0 ){
                absMidy -= mpOrigin.y;
            }
            if( mpOrigin.y < -_contentNode->getBoundingBox().size.height + winSize.height ){
                absMidy +=  -_contentNode->getBoundingBox().size.height + winSize.height - mpOrigin.y;
            }
            // 重设bgSprite锚点和位置
            _contentNode->setAnchorPoint(Vec2(anchorX, anchorY));
            _contentNode->setPosition(Vec2(absMidx, absMidy));
            // 根据两触摸点前后的距离计算缩放倍率
#define RATIO 1.0f
            auto scale = _contentNode->getScale() * (currDistance / prevDistance);
            // 控制缩放倍率在1～4倍之间，最小倍率不能太小，不让背景将不能填充满整个屏幕。
            scale = MIN(4, MAX(1, scale));
            /*
             if (scalex < 1.0) {
             scalex = 1.0;
             }
             else if (scalex > 3.0){
             scalex = 3.0;
             }
             if (scaley < 1.0) {
             scaley = 1.0;
             }
             else if (scaley > 3.0){
             scaley = 3.0;
             }*/
            _contentNode->setScale(scale);
            // 更新原点位置
            mpOrigin = Vec2(absMidx, absMidy) - Vec2(_contentNode->getBoundingBox().size.width * anchorX, _contentNode->getBoundingBox().size.height * anchorY) ;
        }
        else if(touches.size() == 1){// 单点进行移动
            // 单点时，touches中只有一个Touch对象，所以通过touches[0]就可以得到触摸对象
            auto touch = touches[0];
            // 计算滑动过程中的滑动增量
            auto diff = touch->getDelta();
            // 得到当前maplayer的位置
            auto currentPos = _contentNode->getPosition();
            // 得到滑动后maplayer应该所在的位置
            auto pos = currentPos + diff;
            // 得到此刻maplayer的尺寸
            auto maplayerCurrSize = _contentNode->getBoundingBox().size;
            
            //边界控制，约束pos的位置
            pos.x = MIN(pos.x, maplayerCurrSize.width * _contentNode->getAnchorPoint().x);
            pos.x = MAX(pos.x, -maplayerCurrSize.width + winSize.width + maplayerCurrSize.width * _contentNode->getAnchorPoint().x);
            pos.y = MIN(pos.y, maplayerCurrSize.height * _contentNode->getAnchorPoint().y);
            pos.y = MAX(pos.y, -maplayerCurrSize.height + winSize.height + maplayerCurrSize.height * _contentNode->getAnchorPoint().y);
            // 重设bgSprite位置
            _contentNode->setPosition(pos);
            
            // 更新原点位置
            if( pos.x >= maplayerCurrSize.width * _contentNode->getAnchorPoint().x
               || pos.x <= -maplayerCurrSize.width + winSize.width + maplayerCurrSize.width * _contentNode->getAnchorPoint().x){
                diff.x = 0;
            }
            if( pos.y >= maplayerCurrSize.height * _contentNode->getAnchorPoint().y
               || pos.y <= -maplayerCurrSize.height + winSize.height + maplayerCurrSize.height * _contentNode->getAnchorPoint().y){
                diff.y = 0;
            }
            mpOrigin += diff;
        }
    };
    listener->onTouchesEnded = [this](const std::vector<Touch*>&pTouches, Event*pEvent)->void{
        
    };
    this->getEventDispatcher()->addEventListenerWithFixedPriority(listener, 1);
    
    return true;
}