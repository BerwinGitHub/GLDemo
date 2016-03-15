//
//  VisualEffectLayer.cpp
//  Follow
//
//  Created by 唐博文 on 15/12/30.
//
//

#include "VisualEffectLayer.hpp"

VisualEffectLayer::VisualEffectLayer():
_iFrameRate(0),
_fRadius(80.0f),
_pBlurSprite(nullptr)
{
    
}

VisualEffectLayer::~VisualEffectLayer()
{
    
}

VisualEffectLayer* VisualEffectLayer::createWithRadius(const Size &size, int radius)
{
    VisualEffectLayer* pRet = new VisualEffectLayer();
    if (pRet && pRet->initWithRadius(size, radius)) {
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_RELEASE_NULL(pRet);
    return pRet;
}

bool VisualEffectLayer::initWithRadius(const Size &size, int radius)
{
    if (!Node::init()) {
        return false;
    }
    this->setContentSize(size);
    _fRadius = radius;
    this->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    
//    _pBlurSprite = Sprite::create();
//    this->addChild(_pBlurSprite, 0);
//    this->scheduleUpdate();
    
    return true;
}

void prepareCommand(float zorder, Rect worldBoundingBox, std::function<void(unsigned char *, int, int)> func)
{
    
    static CustomCommand captureScreenCommand;
    captureScreenCommand.init(zorder);
    captureScreenCommand.func = std::bind(onCaptureScreen, func, worldBoundingBox);
    Director::getInstance()->getRenderer()->addCommand(&captureScreenCommand);
}

void VisualEffectLayer::draw(cocos2d::Renderer *renderer, const cocos2d::Mat4 &transform, uint32_t flags)
{
    if (++_iFrameRate >= 1) {
        _iFrameRate = 0;
        Rect bb = RectApplyAffineTransform(Rect(0, 0, _contentSize.width, _contentSize.height), this->getNodeToWorldAffineTransform());
        prepareCommand(_globalZOrder + _localZOrder, bb, CC_CALLBACK_3(VisualEffectLayer::captureFinish, this));
    }
    _customCommand.init(_globalZOrder);
    _customCommand.func = CC_CALLBACK_0(VisualEffectLayer::onDraw, this, transform, flags);
    renderer->addCommand(&_customCommand);
    renderer->render();
    
}

void VisualEffectLayer::onDraw(const cocos2d::Mat4 &transform, uint32_t flags)
{
//    Director* director = Director::getInstance();
//    director->pushMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
//    director->loadMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW, _modelViewTransform);
//    Renderer* renderer = director->getRenderer();
//    
//    
//    director->popMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
}

void onCaptureScreen(const std::function<void (unsigned char*, int w, int h)> func, Rect &bb)
{
//    utils::captureScreen(nullptr, "");
//    Rect bb = this->getBoundingBox();
    int x = static_cast<int>(bb.origin.x);
    int y = static_cast<int>(bb.origin.y);
    int w = static_cast<int>(bb.size.width);
    int h = static_cast<int>(bb.size.height);
    int wh4 = w * h << 2;
    int w4 = w << 2;
    
    do {
        std::shared_ptr<GLubyte> buffer(new GLubyte[wh4], [](GLubyte* p){ CC_SAFE_DELETE_ARRAY(p); });
        if (!buffer) {
            break;
        }
        
        glPixelStorei(GL_PACK_ALIGNMENT, 1);
        
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
        // The frame buffer is always created with portrait orientation on WP8.
        // So if the current device orientation is landscape, we need to rotate the frame buffer.
        auto renderTargetSize = glView->getRenerTargetSize();
        CCASSERT(w * h == static_cast<int>(renderTargetSize.width * renderTargetSize.height), "The frame size is not matched");
        glReadPixels(x, y, (int)renderTargetSize.width, (int)renderTargetSize.height, GL_RGBA, GL_UNSIGNED_BYTE, buffer.get());
#else
        glReadPixels(x, y, w, h, GL_RGBA, GL_UNSIGNED_BYTE, buffer.get());
#endif
        
        std::shared_ptr<GLubyte> flippedBuffer(new GLubyte[wh4], [](GLubyte* p) { CC_SAFE_DELETE_ARRAY(p); });
        if (!flippedBuffer) {
            break;
        }
        
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
        int h4 = h << 2;
        if (w == static_cast<int>(renderTargetSize.width)) {
            // The current device orientation is portrait.
            for (int row = 0; row < h; ++row) {
                memcpy(flippedBuffer.get() + (h - row - 1) * w4, buffer.get() + row * w4, w4);
            }
        }else {
            // The current device orientation is landscape.
            for (int row = 0; row < w; ++row) {
                for (int col = 0; col < h; ++col) {
                    *(int*)(flippedBuffer.get() + (h - col - 1) * h4 + row << 2) = *(int*)(buffer.get() + row * h4 + col << 2);
                }
            }
        }
#else
        for (int row = 0; row < h; ++row) {
            memcpy(flippedBuffer.get() + (h - row - 1) * w4, buffer.get() + row * w4, w4);
        }
#endif
        
        unsigned char *data = flippedBuffer.get();
        if (func) {
            func(data, w, h);
        }
//        Director::getInstance()->getRenderer()->render();
        
    }while(0);
}

void VisualEffectLayer::captureFinish(unsigned char *data, int w, int h)
{
    stackblur(data, w, h, _fRadius, 4);

    Texture2D* replaceTexture = new Texture2D();
    replaceTexture->autorelease();
    replaceTexture->initWithData(data, w * h << 2, Texture2D::PixelFormat::RGBA8888, w, h, _contentSize);
    
    if (!_pBlurSprite) {
        _pBlurSprite = Sprite::createWithTexture(replaceTexture);
//        _pBlurSprite->setFlippedY(true);
        this->addChild(_pBlurSprite);
    }else{
        _pBlurSprite->setTexture(replaceTexture);
    }
    _pBlurSprite->setPosition(_contentSize * 0.5f);
//    _pBlurSprite->setOpacity(240);
}

void VisualEffectLayer::update(float dt)
{
//    _fTime += dt;
//    if(_fTime <= 2.0f){
//        return;
//    }
//    _fTime = 0;
//    Rect bb = RectApplyAffineTransform(Rect(0, 0, _contentSize.width, _contentSize.height), this->getNodeToWorldAffineTransform());
//    prepareCommand(_globalZOrder, bb, CC_CALLBACK_3(VisualEffectLayer::captureFinish, this));
}

void VisualEffectLayer::onEnter()
{
    Node::onEnter();
    Rect bb = RectApplyAffineTransform(Rect(0, 0, _contentSize.width, _contentSize.height), this->getNodeToWorldAffineTransform());
    prepareCommand(_globalZOrder + 0.01, bb, CC_CALLBACK_3(VisualEffectLayer::captureFinish, this));
}

void VisualEffectLayer::onExit()
{
    Node::onExit();
}
