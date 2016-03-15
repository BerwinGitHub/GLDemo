//
//  VisualEffectLayer.cpp
//  Follow
//
//  Created by 唐博文 on 15/12/30.
//
//

#include "VisualEffectLayer.hpp"


const static GLfloat GL_COORDS[] = {
    0.0f, 1.0f,
    1.0f, 1.0f,
    0.0f, 0.0f,
    1.0f, 0.0f,
};

const static GLfloat GL_COLOR[] = {
    1.0f, 1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f, 1.0f,
};

VisualEffectLayer::VisualEffectLayer():
_iFrameRate(0),
_fRadius(80.0)
//_pBlurSprite(nullptr)
{
    
}

VisualEffectLayer::~VisualEffectLayer()
{
    _pBlurTexture->release();
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
    
    _pBlurTexture = new Texture2D();
    _pBlurTexture->autorelease();
    _pBlurTexture->retain();
    
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
        prepareCommand(_globalZOrder == 0 ? _localZOrder : _globalZOrder, bb, CC_CALLBACK_3(VisualEffectLayer::captureFinish, this));
    }
    _customCommand.init(_globalZOrder);
    _customCommand.func = CC_CALLBACK_0(VisualEffectLayer::onDraw, this, transform, flags);
    renderer->addCommand(&_customCommand);
    renderer->render();
    
}

void VisualEffectLayer::onDraw(const cocos2d::Mat4 &transform, uint32_t flags)
{
    Rect bb = RectApplyAffineTransform(Rect(0, 0, _contentSize.width, _contentSize.height), this->getNodeToWorldAffineTransform());
    Size s = Size(_pBlurTexture->getPixelsWide(), _pBlurTexture->getPixelsHigh());
    float x = bb.origin.x - (s.width * _anchorPoint.x);
    float y = bb.origin.y - (s.height * _anchorPoint.y);
    // 因为截图的时候y轴是反得,所以这是3,4,1,2 默认是1,2,3,4
    const static GLfloat GL_VERTEX[] = {
        x,              y + s.height,   0.0f,   // lt 3
        x + s.width,    y + s.height,   0.0f,   // rt 4
        x,              y,              0.0f,   // lb 1
        x + s.width,    y,              0.0f,   // rb 2
    };
    
    
    GL::enableVertexAttribs(GL::VERTEX_ATTRIB_FLAG_POS_COLOR_TEX);
    _pBlurTexture->getGLProgram()->use();
    _pBlurTexture->getGLProgram()->setUniformsForBuiltins();
    
    glBindTexture(GL_TEXTURE_2D, _pBlurTexture->getName());
    
    glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 3, GL_FLOAT, GL_FALSE, 0, GL_VERTEX);
    glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_COLOR, 4, GL_FLOAT, GL_FALSE, 0, GL_COLOR);
    glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_TEX_COORDS, 2, GL_FLOAT, GL_FALSE, 0, GL_COORDS);
    
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    
}

void onCaptureScreen(const std::function<void (unsigned char*, int w, int h)> func, Rect &bb)
{
    int x = static_cast<int>(bb.origin.x);
    int y = static_cast<int>(bb.origin.y);
    int w = static_cast<int>(bb.size.width);
    int h = static_cast<int>(bb.size.height);
    int wh4 = w * h << 2;
    
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
        glReadPixels(x, y, w, h, GL_RGBA, GL_UNSIGNED_BYTE, buffer.get());// 很费时间,需要优化
#endif
        
//        std::shared_ptr<GLubyte> flippedBuffer(new GLubyte[wh4], [](GLubyte* p) { CC_SAFE_DELETE_ARRAY(p); });
//        if (!flippedBuffer) {
//            break;
//        }
//        
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
//        int h4 = h << 2;
//        if (w == static_cast<int>(renderTargetSize.width)) {
//            // The current device orientation is portrait.
//            for (int row = 0; row < h; ++row) {
//                memcpy(flippedBuffer.get() + (h - row - 1) * w4, buffer.get() + row * w4, w4);
//            }
//        }else {
//            // The current device orientation is landscape.
//            for (int row = 0; row < w; ++row) {
//                for (int col = 0; col < h; ++col) {
//                    *(int*)(flippedBuffer.get() + (h - col - 1) * h4 + row << 2) = *(int*)(buffer.get() + row * h4 + col << 2);
//                }
//            }
//        }
//#else
//        int w4 = w << 2;
//        for (int row = 0; row < h; ++row) {
//            memcpy(flippedBuffer.get() + (h - row - 1) * w4, buffer.get() + row * w4, w4);
//        }
//#endif
        
//        unsigned char *data = buffer.get();
        if (func) {
            func(buffer.get(), w, h);
        }
        
    }while(0);
}

void VisualEffectLayer::captureFinish(unsigned char *data, int w, int h)
{
    stackblur(data, w, h, _fRadius, 4);
    _pBlurTexture->initWithData(data, w * h << 2, Texture2D::PixelFormat::RGBA8888, w, h, _contentSize);

//    Texture2D* replaceTexture = new Texture2D();
//    replaceTexture->autorelease();
//    replaceTexture->initWithData(data, w * h << 2, Texture2D::PixelFormat::RGBA8888, w, h, _contentSize);
//    
//    if (!_pBlurSprite) {
//        _pBlurSprite = Sprite::createWithTexture(replaceTexture);
//        this->addChild(_pBlurSprite);
//    }else{
//        _pBlurSprite->setTexture(replaceTexture);
//    }
//    _pBlurSprite->setPosition(_contentSize * 0.5f);
}

void VisualEffectLayer::onEnter()
{
    Node::onEnter();
//    Rect bb = RectApplyAffineTransform(Rect(0, 0, _contentSize.width, _contentSize.height), this->getNodeToWorldAffineTransform());
//    prepareCommand(_globalZOrder + 0.01, bb, CC_CALLBACK_3(VisualEffectLayer::captureFinish, this));
}

void VisualEffectLayer::onExit()
{
    Node::onExit();
}
