//
//  DragSprite.cpp
//  Demo
//
//  Created by tangbowen on 8/3/16.
//
//

#include "DragSprite.hpp"

DragSprite::DragSprite():
_fnDelegate(nullptr)
{
    
}

DragSprite::~DragSprite()
{
    this->clearAllResponseNodes();
}

DragSprite* DragSprite::createWithResponseNode(const string &file, cocos2d::Node *rNode)
{
    DragSprite *pRet = new DragSprite();
    if (pRet && pRet->initWithResponseNode(file, rNode)) {
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_RELEASE_NULL(pRet);
    return pRet;
}

bool DragSprite::initWithResponseNode(const string &file, cocos2d::Node *rNode)
{
    if (!Sprite::initWithFile(file)) {
        return false;
    }
    this->addResponseNode(rNode);
    
    this->registerEvent();
    
    return true;
}

void DragSprite::registerEvent()
{
    EventListenerTouchOneByOne *e = EventListenerTouchOneByOne::create();
    e->setSwallowTouches(true);
    e->onTouchBegan = CC_CALLBACK_2(DragSprite::onTouchBegan, this);
    e->onTouchMoved = CC_CALLBACK_2(DragSprite::onTouchMoved, this);
    e->onTouchEnded = CC_CALLBACK_2(DragSprite::onTouchEnded, this);
    e->onTouchCancelled = CC_CALLBACK_2(DragSprite::onTouchCanceled, this);
//  method 1
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(e, this);
//  method 2
//  _eventDispatcher->addEventListenerWithSceneGraphPriority(e, this);
}

bool DragSprite::isAncestorsVisible(Node* node)
{
    if (!node)
        return true;
    Node* parent = node->getParent();
    if (!node->isVisible() || (parent && !parent->isVisible())){
        return false;
    }
    return this->isAncestorsVisible(parent);
}

bool DragSprite::onTouchBegan(Touch *t, Event *e)
{
    if (!this->isAncestorsVisible(this)) {
        return false;
    }
//  method 1
//  Vec2 realPos = this->getParent()->convertToNodeSpace(t->getLocation());
    
//  method 2
//    boundingBox.origin = this->convertToWorldSpace(Vec2::ZERO);
//    boundingBox.size = this->getContentSize();
//  method 3 和2的理解一样,只是换了一种方式转换
    Rect boundingBox = this->convertBoundingBoxToWorld(this);
    if (boundingBox.containsPoint(t->getLocation())) {
        return true;
    }
    return false;
}

void DragSprite::onTouchMoved(Touch *t, Event *e)
{
    this->setPosition(this->getPosition() + t->getDelta());
    Rect boundingBox = Rect::ZERO;
    boundingBox.origin = this->convertToWorldSpace(Vec2::ZERO);
    boundingBox.size = this->getContentSize();
    
    vector<RectNode> vRectNodesRet;
    vector<DragSprite::RectNode> vRectNodes = this->intersectsNodesRect(boundingBox);
    for (auto it = _vResponseRectNodes.begin(); it != _vResponseRectNodes.end(); it++) {
        if (this->isInExistRect((*it), vRectNodes)) {// 判断所有的节点是否在已经的区域内,如果在,就是进入或者已经进入进行移动
            if ((*it).type == EventType::EXIT) {// 表示刚刚进来
                (*it).type = EventType::ENTER;
            }else if((*it).type == EventType::ENTER || (*it).type == EventType::MOVED){// 表示已经进来并移动
                (*it).type = EventType::MOVED;
            }
            vRectNodesRet.push_back((*it));
        }else {// 所有区域中没在已在的矩形区域内,如果实在移动,或者进入的状态那么就表示刚出去.否则表示没有进入过,不需要操作
            if((*it).type != EventType::EXIT){
                // 这里表示这个node前面在里面,在这出去了
                (*it).type = EventType::EXIT;
                vRectNodesRet.push_back((*it));
            }
        }
    }
    
    if(_fnDelegate){ // 通知回调, 已经在某个节点里面, 并把这个节点传过去
        _fnDelegate(this, vRectNodesRet);
    }
}

void DragSprite::onTouchEnded(Touch *t, Event *e)
{
    this->setColor(Color3B(255, 255, 255));
}

void DragSprite::onTouchCanceled(Touch *t, Event *e)
{
    this->onTouchEnded(t, e);
}

vector<DragSprite::RectNode> DragSprite::intersectsNodesRect(const Rect &rect)
{
    vector<RectNode> vRectNodes;
    // 遍历_vResponseNodes
    for(auto it : _vResponseRectNodes){
        Rect boundingBox = this->convertBoundingBoxToWorld(it.node);
        if (boundingBox.intersectsRect(rect)) {
            vRectNodes.push_back(it);
        }
    }
    return vRectNodes;
}

bool DragSprite::isInExistRect(RectNode &rn, vector<RectNode> &v)
{
    for (auto it = v.begin(); it != v.end(); it++) {
        if ((*it).node == rn.node) {
            return true;
        }
    }
    return false;
}

Rect DragSprite::convertBoundingBoxToWorld(cocos2d::Node *n)
{
    if (!n)
        return Rect::ZERO;
    Rect boundingBox = RectApplyAffineTransform(Rect(0, 0, n->getContentSize().width, n->getContentSize().height), n->nodeToWorldTransform());
    return boundingBox;
}

void DragSprite::addResponseNode(cocos2d::Node *n)
{
    if (!n)
        return;
    if (!this->isContainNode(n)) { // 没有包含这个节点就添加
        n->retain(); // 析构里面释放掉
        _vResponseRectNodes.push_back(RectNode{n, DragSprite::EventType::EXIT});
    }
}

void DragSprite::removeResponseNode(cocos2d::Node *n)
{
    for(auto it = _vResponseRectNodes.begin(); it != _vResponseRectNodes.end(); it++){
        if ((*it).node && (*it).node == n) {
            (*it).node->release();
            _vResponseRectNodes.erase(it);
            return;
        }
    }
}

void DragSprite::clearAllResponseNodes()
{
    for(auto it : _vResponseRectNodes){
        it.node->release();
    }
    _vResponseRectNodes.clear();
}

bool DragSprite::isContainNode(cocos2d::Node *n)
{
    for(auto it : _vResponseRectNodes){
        if (it.node == n)
            return true;
    }
    return false;
}

void DragSprite::onEnter()
{
    Sprite::onEnter();
}

void DragSprite::onExit()
{
    Sprite::onExit();
}