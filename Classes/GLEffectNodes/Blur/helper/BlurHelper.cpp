//
//  BlurHelper.cpp
//  Follow
//
//  Created by 唐博文 on 15/12/28.
//
//

#include "BlurHelper.h"
#include "StackBlurTask.h"

Sprite* BlurHelper::blurNodeRect(cocos2d::Node *target, cocos2d::Rect rect, int blurRadius/* = 50*/)
{
    if (!target) {
        return nullptr;
    }
    RenderTexture* render = RenderTexture::create(rect.size.width, rect.size.height, Texture2D::PixelFormat::RGBA8888, GL_DEPTH24_STENCIL8);
    Vec2 pos = target->getPosition();
    target->setPosition(pos - rect.origin);
    render->beginWithClear(0, 0, 0, 0);
    target->visit();
    render->end();
    target->setPosition(pos);
    Director::getInstance()->getRenderer()->render();
    render->getSprite()->getTexture()->setAntiAliasTexParameters();
    render->getSprite()->setFlippedY(true);
    
    Image *image = render->newImage();
    unsigned char *data = image->getData();
    unsigned int w = image->getWidth();
    unsigned int h = image->getHeight();
    ssize_t dataLen = image->getDataLen();
    
    stackblur(data, w, h, blurRadius, 4);
    
    Texture2D* texture = new Texture2D();
    texture->autorelease();
    texture->initWithData(data, dataLen, Texture2D::PixelFormat::RGBA8888, w, h, Size(w, h));
    texture->setAntiAliasTexParameters();
    Sprite* screenSpr = Sprite::createWithTexture(texture);
    
    delete image;
    //    delete data;
    
    return screenSpr;
}

Sprite* BlurHelper::blurScreen(int blurRadius/* = 50*/)
{
    Scene* runningScene = Director::getInstance()->getRunningScene();
    if (!runningScene) {
        return nullptr;
    }
    return BlurHelper::blurNodeRect(runningScene, runningScene->getBoundingBox(), blurRadius);
}

Sprite* BlurHelper::blurScreenNode(cocos2d::Node *target, int blurRadius/* = 50*/)
{
    return BlurHelper::blurNodeRect(target, target->getBoundingBox());
}

Sprite* BlurHelper::blurScreenNodeWithClipping(cocos2d::Node *target, Vec2 origin, int blurRadius/* = 50*/)
{
    Scene* runningScene = Director::getInstance()->getRunningScene();
    if (!runningScene) {
        return nullptr;
    }
    Rect rect = Rect(origin.x, origin.y, target->getContentSize().width, target->getContentSize().height);
    Sprite* screen = BlurHelper::renderNodeRect(runningScene, rect);
    Sprite* blur = BlurHelper::blurNodeRect(screen, screen->getBoundingBox(), blurRadius);
    
    ClippingNode* clipping = ClippingNode::create(target);
    target->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    clipping->setContentSize(target->getContentSize());
    clipping->setAlphaThreshold(0.1f);
    blur->setAnchorPoint(Vec2::ZERO);
    blur->setPosition(Vec2::ZERO);
    clipping->addChild(blur);
    
    Sprite* spr = BlurHelper::renderNodeRect(clipping, target->getBoundingBox());
    return spr;
}



Sprite* BlurHelper::renderNodeRect(cocos2d::Node *target, cocos2d::Rect rect)
{
    if (!target) {
        return nullptr;
    }
    RenderTexture* render = RenderTexture::create(rect.size.width, rect.size.height, Texture2D::PixelFormat::RGBA8888, GL_DEPTH24_STENCIL8);
    Vec2 pos = target->getPosition();
    target->setPosition(pos - rect.origin);
    render->beginWithClear(0, 0, 0, 0);
    target->visit();
    render->end();
    target->setPosition(pos);
    Director::getInstance()->getRenderer()->render();
    render->getSprite()->getTexture()->setAntiAliasTexParameters();
    
    Sprite* renderSpr = Sprite::createWithSpriteFrame(render->getSprite()->getSpriteFrame());
    renderSpr->setFlippedY(true);
    return renderSpr;
}

BlurHelper::BlurHelper()
{
    
}

BlurHelper::~BlurHelper()
{
    
}