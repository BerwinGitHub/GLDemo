//
//  StackBlurSprite.cpp
//  JniHelper
//
//  Created by tangbowen on 27/11/15.
//
//

#include "StackBlurSprite.h"
#include "StackBlurTask.h"
#include <string.h>

StackBlurSprite::StackBlurSprite():
_originalData(nullptr)
{
    
}

StackBlurSprite::~StackBlurSprite()
{
    delete _originalData;
}

StackBlurSprite* StackBlurSprite::createWithRadius(const string &file, int radius)
{
    StackBlurSprite* pRet = new StackBlurSprite();
    if (pRet && pRet->initWithRadius(file, radius)) {
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_RELEASE_NULL(pRet);
    return nullptr;
}

bool StackBlurSprite::initWithRadius(const string &file, int radius)
{
    if(!Sprite::initWithFile(file)){
        return false;
    }
    
    _iRadius = radius;
    this->initImageData();
    this->changeRadiusTexture();
    
    return true;
}

void StackBlurSprite::initImageData()
{
    RenderTexture* renderTexture = RenderTexture::create(this->getContentSize().width, this->getContentSize().height, Texture2D::PixelFormat::RGBA8888, GL_DEPTH24_STENCIL8);
    Vec2 pos = _position;
    Vec2 anchor = _anchorPoint;
    this->setPosition(Vec2::ZERO);
    this->setAnchorPoint(Vec2::ZERO);
    renderTexture->begin();
    this->visit();
    renderTexture->end();
    this->setPosition(pos);
    this->setAnchorPoint(anchor);
    Director::getInstance()->getRenderer()->render();
    Image* img = renderTexture->newImage();
    
    _originalDataLen = img->getDataLen();
    _originalData = img->getData();
    _pixelsWide = img->getWidth();
    _pixelsHigh = img->getHeight();
    
}

void StackBlurSprite::changeRadiusTexture()
{
    unsigned char* blurData = static_cast<unsigned char*>(malloc(_originalDataLen * sizeof(unsigned char)));
    memcpy(blurData, _originalData, _originalDataLen);
    
    stackblur(blurData, _pixelsWide, _pixelsHigh, _iRadius, 4);
    
    Texture2D* replaceTexture = new Texture2D();
    replaceTexture->autorelease();
    replaceTexture->initWithData(blurData, _originalDataLen, Texture2D::PixelFormat::RGBA8888, _pixelsWide, _pixelsHigh, this->getContentSize());
    this->setTexture(replaceTexture);
    delete blurData;
}

void StackBlurSprite::setBlurRadius(int radius)
{
    if (radius < 1 || radius > 254) {
        return;
    }
    _iRadius = radius;
    this->changeRadiusTexture();
}

int StackBlurSprite::getBlurRadius()
{
    return _iRadius;
}

void StackBlurSprite::onEnter()
{
    Sprite::onEnter();
}

void StackBlurSprite::onExit()
{
    Sprite::onExit();
}