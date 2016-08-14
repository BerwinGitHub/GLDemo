//
//  ScribbleArmatureNode.cpp
//  ScribbleDemo_3_2
//
//  Created by renhong on 14-10-29.
//
//

#include "ScribbleArmatureNode.h"
#include "cocostudio/CCArmature.h"

ScribbleArmatureNode *ScribbleArmatureNode::create(Size pCanvasSize){
    ScribbleArmatureNode *ret = new ScribbleArmatureNode();
    if(ret && ret->init(pCanvasSize)){
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool ScribbleArmatureNode::init(Size pCanvasSize){
    if ( !ScribbleTouchNode::init(pCanvasSize) ){
        return false;
    }
    
    _touchListener->setSwallowTouches(false);
    return true;
}

ScribbleArmatureNode::ScribbleArmatureNode() : _armaturePosition(0,0), _bone(nullptr), _skinTransform(Mat4::IDENTITY), _haveArmaturePosition(false){
}

void ScribbleArmatureNode::paint(Point pWorldPosition){
    ScribbleTouchNode::paint(this->positionInArmature(pWorldPosition));
}

void ScribbleArmatureNode::paint(Point pStartWorldPosition, Point pEndWorldPosition){
    ScribbleTouchNode::paint(this->positionInArmature(pStartWorldPosition), this->positionInArmature(pEndWorldPosition));
}

void ScribbleArmatureNode::paint(vector<Point> pWorldPositions){
    vector<Point> lPoints;
    for (vector<Point>::iterator lItr = pWorldPositions.begin(); lItr != pWorldPositions.end(); ++lItr) {
        lPoints.push_back(this->positionInArmature(*lItr));
    }
    ScribbleTouchNode::paint(lPoints);
}

void ScribbleArmatureNode::setAdditionalTransform(Mat4* additionalTransform){
    CCASSERT(nullptr != _bone, "bone can not be null");
    if (nullptr != _bone) {
        _transform = TransformConcat(_bone->getNodeToArmatureTransform(), _skinTransform);
    }
}

void ScribbleArmatureNode::setSkinData(const BaseData &var){
    _skinData = var;
    
    setScaleX(_skinData.scaleX);
    setScaleY(_skinData.scaleY);
    setRotationSkewX(CC_RADIANS_TO_DEGREES(_skinData.skewX));
    setRotationSkewY(CC_RADIANS_TO_DEGREES(-_skinData.skewY));
    setPosition(Vec2(_skinData.x, _skinData.y));
    
    _skinTransform = getNodeToParentTransform();
    updateArmatureTransform();
}

const BaseData &ScribbleArmatureNode::getSkinData() const{
    return _skinData;
}

void ScribbleArmatureNode::visit(Renderer* renderer, const Mat4 &parentTransform, uint32_t parentFlags){
    // quick return if not visible. children won't be drawn.
    if (!_visible){
        return;
    }
    
    uint32_t flags = processParentFlags(parentTransform, parentFlags);
    
    // IMPORTANT:
    // To ease the migration to v3.0, we still support the Mat4 stack,
    // but it is deprecated and your code should not rely on it
    Director* director = Director::getInstance();
    director->pushMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
    director->loadMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW, _modelViewTransform);
    
    int i = 0;
    
    if(!_children.empty()){
        sortAllChildren();
        // draw children zOrder < 0
        for( ; i < _children.size(); i++ ){
            auto node = _children.at(i);
            
            if ( node && node->getLocalZOrder() < 0 )
                node->visit(renderer, _modelViewTransform, flags);
            else
                break;
        }
        // self draw
        this->draw(renderer, _modelViewTransform, flags);
        //transform self
        this->updateTransform();
        for(auto it=_children.cbegin()+i; it != _children.cend(); ++it)
            (*it)->visit(renderer, _modelViewTransform, flags);
    }
    else{
        this->draw(renderer, _modelViewTransform, flags);
    }
    
    director->popMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
    
    // FIX ME: Why need to set _orderOfArrival to 0??
    // Please refer to https://github.com/cocos2d/cocos2d-x/pull/6920
    // reset for next frame
    // _orderOfArrival = 0;
}

void ScribbleArmatureNode::updateArmatureTransform(){
    CCASSERT(nullptr != _bone, "bone can not be null");
    if (nullptr != _bone) {
        _transform = TransformConcat(_bone->getNodeToArmatureTransform(), _skinTransform);
    }
}

void ScribbleArmatureNode::updateTransform(){
    // If it is not visible, or one of its ancestors is not visible, then do nothing:
    if( !_visible){
        //_quad.br.vertices = _quad.tl.vertices = _quad.tr.vertices = _quad.bl.vertices = Vec3(0, 0, 0);
        this->setScale(0.0f);
    }
    else{
        Mat4 transform = getNodeToParentTransform();
        this->setPosition(Point(transform.m[12], transform.m[13]));
        this->setRotation(CC_RADIANS_TO_DEGREES(atan(transform.m[1] / transform.m[0])));
        this->getCanvas()->getNodeToWorldTransform();//设置canvas相对于世界坐标系的坐标变换
    }
}

void ScribbleArmatureNode::setBone(Bone *bone){
    CCASSERT(nullptr != bone, "bone can not be null");
    _bone = bone;
    Armature *armature = _bone->getArmature();
    if(armature){
//        _armature = armature;
        SpriteDisplayData *lDisplayData = dynamic_cast<SpriteDisplayData *>(bone->getBoneData());
        if (nullptr != lDisplayData) {
            BaseData lSkinData = lDisplayData->skinData;
            this->setSkinData(lSkinData);
        }
    }
}

Bone *ScribbleArmatureNode::getBone() const{
    return _bone;
}
