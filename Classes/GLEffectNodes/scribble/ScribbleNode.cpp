//
//  ScribbleNode.cpp
//  ScribbleDemo_3_2
//
//  Created by renhong on 8/4/14.
//
//

#include "ScribbleNode.h"
#include "ScribbleUtil.h"

ScribbleNode *ScribbleNode::create(Size pCanvasSize){
    ScribbleNode *ret = new ScribbleNode();
    if(ret && ret->init(pCanvasSize))
    {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

vector<Point> ScribbleNode::interpolate(Point pStartPosition, Point pEndPosition, float pDelta){
    return ScribbleUtil::getInstance()->interpolate(pStartPosition, pEndPosition, pDelta);
}

vector<Point> ScribbleNode::waterPositions(Point pFromPoint, float pMaxX, float pDelta, Vec2 pDirection){
    float lVy = pDirection.y;
    float lVx = pDirection.x;
    const float lAccelerationRate = 0.01;
    int lXVar = pMaxX;
    int a = 0;
    float lX = pFromPoint.x;
    float lY = pFromPoint.y;
    vector<Point> lPositions;
    for (int i = 0; i < lXVar; i += pDelta) {
        Point lPosition;
        a += pDelta;
        lPosition.x = lX + lVx * i;
        lPosition.y = lY + lVy * i - lAccelerationRate * a * a;
        lPositions.push_back(lPosition);
        
        //CCLOG("touchmove2 %f %f %d", lX, lY, a);
    }
    return lPositions;
}

ScribbleNode::ScribbleNode() : _brush(nullptr), _canvas(nullptr){
    _scribble = new Scribble(Brush::create());
    _scribble->setBrushShader();
    _scribble->setBrushType(Scribble::eBrush);
    _scribble->setTargetSolid(true);
    //_scribble->setTargetAlphaTestValue(0.1f);
}

ScribbleNode::~ScribbleNode(){
    delete _scribble;
    
    if (nullptr != _brush) {
        _brush->release();
    }
    if (nullptr != _canvas) {
        _canvas->release();
    }
}

bool ScribbleNode::init(Size pCanvasSize){
    if (!Node::init()) {
        return false;
    }
    _canvasSize = pCanvasSize;
    _canvas = Canvas::create(pCanvasSize.width, pCanvasSize.height);
    _canvas->retain();
    //_canvas->clear(1, 0, 0, 0.1);
    this->addChild(_canvas);
    this->setContentSize(pCanvasSize);
    return true;
}

void ScribbleNode::antiAliasing(){
    this->getScribble()->antiAliasing(this->getCanvas());
}

void ScribbleNode::disAntiAliasing(){
    this->getScribble()->antiAliasing(this->getCanvas());
}

void ScribbleNode::setAntiAliasingBrush(bool pAntiAliasingBrush){
    this->getScribble()->setAntiAliasingBrush(pAntiAliasingBrush);
}

bool ScribbleNode::useBrush(string pBrushFile, Scribble::BrushType pBrushType/* = Scribble::eBrush*/, float pScale/* = 1.0*/){
    CCLOG("Warning: Dot use this function frequently");
    if (0 == pBrushFile.length()) {
        return false;
    }
    
    Texture2D *lBrushTexture = nullptr;
    if(fabs(pScale - 1.0) <= 0.001){
        lBrushTexture = Director::getInstance()->getTextureCache()->addImage(pBrushFile);
        CCASSERT(lBrushTexture != nullptr, "brush is null");
    }
    else{
        Sprite *lBrushSprite = Sprite::create(pBrushFile);
        lBrushSprite->setScale(pScale);
        lBrushSprite->setPosition(lBrushSprite->getContentSize() * pScale / 2.0);
        RenderTexture *lRenderTexture = RenderTexture::create(lBrushSprite->getContentSize().width * pScale, lBrushSprite->getContentSize().height * pScale, Texture2D::PixelFormat::RGBA8888, GL_DEPTH24_STENCIL8);
//        lRenderTexture->clear(1, 0, 0, 1);
        lRenderTexture->getSprite()->getTexture()->setAliasTexParameters();
//        Texture2D::TexParams lTexParams = {GL_NEAREST, GL_LINEAR_MIPMAP_LINEAR, GL_NONE, GL_NONE};
//        lRenderTexture->getSprite()->getTexture()->setTexParameters(lTexParams);
        lRenderTexture->begin();
        lBrushSprite->visit();
        lRenderTexture->end();
        lBrushTexture = lRenderTexture->getSprite()->getTexture();
//        this->addChild(lRenderTexture);
//        lRenderTexture->setPosition(-100, -100);
    }
    _scribble->setBrush(lBrushTexture);
    _scribble->setBrushType(pBrushType);
    return true;
}

bool ScribbleNode::useBrush(Scribble::BrushType pBrushType){
    _scribble->setBrushType(pBrushType);
    return true;
}

bool ScribbleNode::addTarget(string pTargetFile, Vec2 pPositionInCanvas){
    if (_targets.find(pTargetFile) != _targets.end()) {
        return true;
    }
    Target *lTarget = Target::create(pTargetFile);
    lTarget->setPosition(pPositionInCanvas);
    _targets.insert(pTargetFile, lTarget);
    return true;
}

bool ScribbleNode::addTarget(string pTargetFile){
    return this->addTarget(pTargetFile, Vec2(_canvasSize.width / 2.0f, _canvasSize.height / 2.0f));
}

bool ScribbleNode::useTarget(string pTargetFile, Vec2 pPositionInCanvas){
    this->addTarget(pTargetFile, pPositionInCanvas);
    Target *lTarget = _targets.find(pTargetFile)->second;
    lTarget->setPosition(pPositionInCanvas);
    _scribble->setTarget(lTarget);
    return true;
}

bool ScribbleNode::useTarget(string pTargetFile){
    this->addTarget(pTargetFile);
    Target *lTarget = _targets.find(pTargetFile)->second;
    _scribble->setTarget(lTarget);
    return true;
}

bool ScribbleNode::useTarget(Sprite *pTarget, Vec2 pPositionInCanvas){
    pTarget->setPosition(pPositionInCanvas);
    _scribble->setTarget(pTarget);
    return true;
}

bool ScribbleNode::useTarget(Sprite *pTarget){
    _scribble->setTarget(pTarget);
    return true;
}

bool ScribbleNode::resetCanvas(){
    this->getCanvas()->getSprite()->setOpacity(255);
    this->getCanvas()->getSprite()->setColor(Color3B::WHITE);
    this->getCanvas()->clear(0, 0, 0, 0);
    
    return true;
}

bool ScribbleNode::setBrushSolid(bool pSolid, float pSolidRate/* = 1.0*/){
    this->getScribble()->setTargetSolid(pSolid, pSolidRate);
    return true;
}

void ScribbleNode::setBrushAlphaTestValue(float pAlphaValue){
    this->getScribble()->setBrushAlphaTestValue(pAlphaValue);
}

//bool ScribbleNode::useTarget(Vec2 pPositionInCanvas){
//    Target *lTarget = Sprite::createWithSpriteFrame(_canvas->getSprite()->getSpriteFrame());
//    lTarget->setPosition(pPositionInCanvas);
//    lTarget->setFlippedY(true);
//    lTarget->getTexture()->
//    _scribble->setTarget(lTarget);
//    return true;
//}

void ScribbleNode::paint(string pPath){
//    this->useTarget(pPath);//指定将要绘制的对象
//    this->useBrush(pPath);//指定用于绘制的画刷。
//    _scribble->paint(nullptr, _canvas, this->convertToWorldSpace(Vec2::ZERO));
    Target *lTarget = Target::create(pPath);
    lTarget->setAnchorPoint(Vec2::ZERO);
    this->getCanvas()->begin();
    lTarget->visit();
    this->getCanvas()->end();
}

void ScribbleNode::paint(Point pPosition){
    _scribble->paint(nullptr, _canvas, pPosition);
}

void ScribbleNode::paint(Point pStartPosition, Point pEndPosition){
    _scribble->paint(nullptr, _canvas, pStartPosition, pEndPosition);
}

void ScribbleNode::paint(vector<Point> pWorldPositions){
    _scribble->paint(nullptr, _canvas, pWorldPositions);
}