//
//  SmearArmature.cpp
//  GameFrame
//
//  Created by 唐博文 on 16/8/7.
//
//

#include "SmearArmature.hpp"
#include "cocostudio/CCSkin.h"
#include "cocostudio/CCArmature.h"
#include "./support/smearMath.hpp"

SmearArmature::SmearArmature()
: _pBone(nullptr)
, _skinTransform(Mat4::IDENTITY)
{
    
}

SmearArmature::~SmearArmature()
{
    
}

SmearArmature* SmearArmature::createWithBone(cocostudio::Bone *b, const Target *t, const Paint *p)
{
    SmearArmature *pRet = new SmearArmature();
    if (pRet && pRet->initWithBone(b, t, p)) {
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_RELEASE_NULL(pRet);
    return pRet;
}

SmearArmature* SmearArmature::createWithBone(cocostudio::Bone *b, const string &tFile, const string &pFile)
{
    return SmearArmature::createWithBone(b, Target::create(tFile), Paint::create(pFile));
}

bool SmearArmature::initWithBone(cocostudio::Bone *b, const Target *t, const Paint *p)
{
    if (!SmearNode::init(t, p)) {
        return false;
    }
    this->setBone(b);
    return true;
}

void SmearArmature::draw(const cocos2d::Vec2 &pos)
{
    this->draw(vector<Vec2>{pos});
}

void SmearArmature::draw(const cocos2d::Vec2 &swp, const cocos2d::Vec2 &ewp)
{
    int delta = _pPaint->getContentSize().width * 0.25f * _pPaint->getScaleX();
    vector<Vec2> vPosition = smearMath::interpolate(swp, ewp, delta);
    this->draw(vPosition);
}

void SmearArmature::draw(vector<cocos2d::Vec2> worldPositions)
{
    vector<Vec2> vPosition;
    for (vector<Vec2>::iterator itr = worldPositions.begin(); itr != worldPositions.end(); ++itr) {
        vPosition.push_back(this->positionInArmature(*itr));
    }
    this->SmearNode::draw(vPosition);
}

void SmearArmature::visit(cocos2d::Renderer *render, const cocos2d::Mat4 &transform, uint32_t flag)
{
    // quick return if not visible. children won't be drawn.
    if (!_visible)
        return;
    
    uint32_t flags = processParentFlags(transform, flag);
    
    // IMPORTANT:
    // To ease the migration to v3.0, we still support the Mat4 stack,
    // but it is deprecated and your code should not rely on it
    Director* director = Director::getInstance();
    director->pushMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
    director->loadMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW, _modelViewTransform);
    
    int i = 0;
    
    if(!_children.empty()){
        sortAllChildren();
        for( ; i < _children.size(); i++ ){
            auto node = _children.at(i);
            if ( node && node->getLocalZOrder() < 0 )
                node->visit(render, _modelViewTransform, flags);
            else
                break;
        }
        this->cocos2d::Node::draw(render, _modelViewTransform, flags);
        this->updateTransform();
        for(auto it=_children.cbegin()+i; it != _children.cend(); ++it)
            (*it)->visit(render, _modelViewTransform, flags);
    }
    else{
        this->cocos2d::Node::draw(render, _modelViewTransform, flags);
    }
    
    director->popMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
}

void SmearArmature::updateTransform()
{
    if( !_visible){
        this->setScale(0.0f);
    } else {
        Mat4 transform = this->getNodeToParentTransform();
        this->setPosition(Vec2(transform.m[12], transform.m[13]));
        this->setRotation(CC_RADIANS_TO_DEGREES(atan(transform.m[1] / transform.m[0])));
//        this->getNodeToWorldTransform();
    }
}

void SmearArmature::setAdditionalTransform(Mat4* additionalTransform)
{
    CCASSERT(_pBone, "Bone can't be null.");
    if (_pBone) {
        _transform = TransformConcat(_pBone->getNodeToArmatureTransform(), _skinTransform);
    }
}

void SmearArmature::setBone(cocostudio::Bone *b)
{
    CCASSERT(b, "Bone can't be null.");
    _pBone = b;
    Armature *armature = b->getArmature();
    if (armature) {
        _armatureTransformInWorld = armature->getNodeToWorldAffineTransform();
        SpriteDisplayData *displayData = dynamic_cast<SpriteDisplayData *>(b->getBoneData());
        if (displayData) {
            BaseData skinData = displayData->skinData;
            this->setSkinData(skinData);
        }
    }
    b->addDisplay(this, 0);
    b->changeDisplayWithIndex(0, true);
}

Bone *SmearArmature::getBone() const
{
    return _pBone;
}

void SmearArmature::setSkinData(const cocostudio::BaseData &data)
{
    _skinData = data;
    
    setScaleX(_skinData.scaleX);
    setScaleY(_skinData.scaleY);
    setRotationSkewX(CC_RADIANS_TO_DEGREES(_skinData.skewX));
    setRotationSkewY(CC_RADIANS_TO_DEGREES(-_skinData.skewY));
    setPosition(Vec2(_skinData.x, _skinData.y));
    
    _skinTransform = getNodeToParentTransform();
    this->updateArmatureTransform();
}

const BaseData &SmearArmature::getSkinData() const
{
    return _skinData;
}

void SmearArmature::updateArmatureTransform()
{
    CCASSERT(_pBone, "Bone can't be null.");
    if (_pBone)
        _transform = TransformConcat(_pBone->getNodeToArmatureTransform(), _skinTransform);
}

Vec2 SmearArmature::positionInArmature(const cocos2d::Vec2 &worldPos)
{
    Vec2 vRet = worldPos;
    Vec2 pos = Vec2(_armatureTransformInWorld.tx, _armatureTransformInWorld.ty);
    vRet.subtract(pos);
    
    Vec2 scale = Vec2(_armatureTransformInWorld.a, _armatureTransformInWorld.d);
    return Vec2(vRet.x / scale.x, vRet.y / scale.y);
}

Target *SmearArmature::getTargetByBone(cocostudio::Bone *b)
{
    Target *target = nullptr;
    if(Sprite *skin = dynamic_cast<Sprite *>(b->getDisplayRenderNode())){
        SpriteFrame *frame = skin->getSpriteFrame();
        target = Target::createWithSpriteFrame(frame);
    } else if(SmearNode *smear = dynamic_cast<SmearNode *>(b->getDisplayRenderNode())){
        SpriteFrame *frame = smear->getTarget()->getSpriteFrame();
        target =  Sprite::createWithSpriteFrame(frame);
    }
    return target;
}

void SmearArmature::onEnter()
{
    SmearNode::onEnter();
}

void SmearArmature::onExit()
{
    SmearNode::onExit();
}
