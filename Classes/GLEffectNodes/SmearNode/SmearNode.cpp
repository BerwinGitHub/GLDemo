//
//  SmearNode.cpp
//  GLDemo
//
//  Created by tangbowen on 16/7/28.
//
//

#include "SmearNode.hpp"
#include "./shader/SmearShaderFrag.h"

SmearNode::SmearNode()
: _pTarget(nullptr)
, _pPaint(nullptr)
, _pCanvas(nullptr)
, _bAntiAliasingPaint(false)
, _ePaintType(PaintType::kPaint)
{
    _pPaint = Paint::create();
    _pPaint->retain();
    _pTarget = Target::create();
    _pTarget->retain();
}

SmearNode::~SmearNode()
{
    CC_SAFE_RELEASE_NULL(_pPaint);
    CC_SAFE_RELEASE_NULL(_pTarget);
    CC_SAFE_RELEASE_NULL(_pCanvas);
}

SmearNode* SmearNode::create(const Target *t, const Paint *p)
{
    SmearNode *pRet = new SmearNode();
    if (pRet && pRet->init(t, p)) {
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_RELEASE_NULL(pRet);
    return nullptr;
}

bool SmearNode::init(const Target *t, const Paint *p)
{
    if (!Node::init()) {
        return false;
    }
    this->initShader();
    this->setPaintTexture(p->getTexture());
    this->setPaintType(SmearNode::PaintType::kPaint);
    
    _pCanvas = Canvas::create(t->getContentSize().width, t->getContentSize().height);
    _pCanvas->retain();
    this->addChild(_pCanvas);
    this->setContentSize(t->getContentSize());
    
    this->setTargetTexture(t->getTexture());
    
    return true;
}

void SmearNode::draw(cocos2d::Vec2 pPosition)
{
    vector<Vec2> lPositions;
    lPositions.push_back(pPosition);
    this->draw(lPositions);
}

void SmearNode::draw(cocos2d::Vec2 pStartPosition, cocos2d::Vec2 pEndPosition)
{
    int lDelta = _pPaint->getContentSize().width / 4.0 * _pPaint->getScaleX();//5.0 * pBrush->getScale();  5.0 * pBrush->getScale();//
    vector<Vec2> lPositions = this->interpolate(pStartPosition, pEndPosition, lDelta);
    this->draw(lPositions);
}

void SmearNode::draw(vector<cocos2d::Vec2> pWorldPositions)
{
    for (int i = 0; i < pWorldPositions.size(); ++i) {
        Vec2 lPosition = _pCanvas->getSprite()->convertToNodeSpace(pWorldPositions.at(i));
        Size lCanvasSize = _pCanvas->getSprite()->getContentSize();
        Rect lCanvasBoundingBox = Rect(0, 0, lCanvasSize.width, lCanvasSize.height);
        if (lCanvasBoundingBox.containsPoint(lPosition)) {
            //set the coord of pen at the target
            this->bindPaintPosition(lPosition);
            //draw
            _pPaint->setPosition(lPosition);
            
            _pCanvas->begin();
            _pPaint->visit();
            _pCanvas->end();
            Director::getInstance()->getRenderer()->render();
        }
    }
}

void SmearNode::setPaintType(SmearNode::PaintType t)
{
    _ePaintType = t;
    switch (_ePaintType) {
        case SmearNode::PaintType::kPaint:{
            this->bindTargetSolid(true, 0.1f);
            BlendFunc f = _bAntiAliasingPaint ? BlendFunc({GL_ONE, GL_ONE_MINUS_SRC_ALPHA }) : BlendFunc({GL_ONE, GL_ZERO});
            _pPaint->setBlendFunc(f);
            break;
        }
//        case eAdditiveBrush:{
//            BlendFunc f = {GL_ONE, GL_ONE_MINUS_SRC_ALPHA};
//            lBrush->setBlendFunc(f);
//            break;
//        }
//        case Scribble::eAdditiveBrush_Src:{
//            BlendFunc f = {GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA};//{GL_ONE, GL_ZERO};//
//            lBrush->setBlendFunc(f);
//            
//            break;
            //        }
        case SmearNode::PaintType::kEaserFade:{
            this->bindTargetSolid(false, 0.1f);
            BlendFunc f = {GL_ZERO, GL_SRC_ALPHA };
            _pPaint->setBlendFunc(f);
            break;
        }
        case SmearNode::PaintType::kEaser:{
            this->bindTargetSolid(false);
            BlendFunc f = {GL_ZERO, GL_SRC_ALPHA };
            _pPaint->setBlendFunc(f);
            break;
        }
        default:
            break;
    }
}

SmearNode::PaintType SmearNode::getPaintType()
{
    return _ePaintType;
}

void SmearNode::setPaintTexture(cocos2d::Texture2D *tex)
{
    SpriteFrame *frame = SpriteFrame::createWithTexture(tex, Rect(0, 0, tex->getContentSize().width, tex->getContentSize().height));
    _pPaint->setSpriteFrame(frame);
    this->bindPaintTexture();
}

void SmearNode::setTargetTexture(cocos2d::Texture2D *tex)
{
    _pTarget->setPosition(_pCanvas->getSprite()->getContentSize() * 0.5f);
    SpriteFrame *frame = SpriteFrame::createWithTexture(tex, Rect(0, 0, tex->getContentSize().width, tex->getContentSize().height));
    _pTarget->setSpriteFrame(frame);
    this->bindTargetTexture();
}

void SmearNode::initShader()
{
    string key = this->createUniqueGLProgramName(kSmearShaderName, this);
    GLProgram *p = this->loadGLProgram(ccPositionTextureColor_noMVP_vert, SmearShadeFrag, key);
    if (p && _pPaint->getGLProgram() != p) {
        this->setGLProgram(_pPaint, p);
        
        this->bindPaintAlphaValue(0.0f);
        this->bindTargetAlphaValue(0.0f);
        this->bindTargetSolid(true);
        this->bindPaintTexture();
    }
}

void SmearNode::bindPaintPosition(cocos2d::Vec2 p)
{
    float lX = (p.x - _pTarget->getPosition().x + _pTarget->getContentSize().width / 2.0 - _pPaint->getContentSize().width / 2.0);
    float lY = _pTarget->getContentSize().height - (p.y - _pTarget->getPosition().y + _pTarget->getContentSize().height / 2.0 + _pPaint->getContentSize().height / 2.0);
    GLProgramState *lState = GLProgramState::getOrCreateWithGLProgram(_pPaint->getGLProgram());
    lState->setUniformVec2("v_texCoord_target", Vec2(lX,lY));
}

void SmearNode::bindPaintAlphaValue(float v)
{
    CCASSERT(v <= 1.0f && v >= 0.0f, "Apha value must in [0.0,1.0]");
    GLProgramState *lState = GLProgramState::getOrCreateWithGLProgram(_pPaint->getGLProgram());
    lState->setUniformFloat("f_alpha_value_paint", v);
}

void SmearNode::bindTargetAlphaValue(float v)
{
    CCASSERT(v <= 1.0f && v >= 0.0f, "Apha value must in [0.0,1.0]");
    GLProgramState *lState = GLProgramState::getOrCreateWithGLProgram(_pPaint->getGLProgram());
    lState->setUniformFloat("f_alpha_value_target", v);
}

void SmearNode::bindTargetSolid(bool s, float r/* = 1.0f*/)
{
    int lIsSolid = s ? 1 : 0;
    GLProgramState *lState = GLProgramState::getOrCreateWithGLProgram(_pPaint->getGLProgram());
    lState->setUniformInt("v_solid_paint", lIsSolid);
    lState->setUniformFloat("v_solid_rate", r);
}

void SmearNode::bindAntiAliasingPaint(bool b)
{
    _bAntiAliasingPaint = b;
    int lAntiAliasingBrush = _bAntiAliasingPaint ? 1 : 0;
    GLProgramState *lState = GLProgramState::getOrCreateWithGLProgram(_pPaint->getGLProgram());
    lState->setUniformInt("anti_aliasing_paint", lAntiAliasingBrush);
}

void SmearNode::bindPaintTexture()
{
    GLProgramState *lState = GLProgramState::getOrCreateWithGLProgram(_pPaint->getGLProgram());
    lState->setUniformVec2("v_texSize_paint", Vec2(_pPaint->getContentSize().width, _pPaint->getContentSize().height));
    lState->setUniformTexture("s_texture_paint", _pPaint->getTexture());
}

void SmearNode::bindTargetTexture()
{
    GLProgramState *lState = GLProgramState::getOrCreateWithGLProgram(_pPaint->getGLProgram());
    lState->setUniformVec2("v_texSize_target", Vec2(_pTarget->getContentSize().width, _pTarget->getContentSize().height));
    lState->setUniformTexture("s_texture_target", _pTarget->getTexture());
}

void SmearNode::onEnter()
{
    Node::onEnter();
}

void SmearNode::onExit()
{
    Node::onExit();
}

string SmearNode::createUniqueGLProgramName(string name, void *p)
{
    char cKey[512];
    sprintf(cKey, "%s%p", name.c_str(), p);
    return string(cKey);
}

GLProgram *SmearNode::loadGLProgram(const GLchar *vert, const GLchar *frag, string key)
{
    GLProgram *p = GLProgram::createWithByteArrays(vert, frag);
    CHECK_GL_ERROR_DEBUG();
    if (nullptr != p) {
        GLProgramCache::getInstance()->addGLProgram(p, key);
    }
    return p;
}

void SmearNode::setGLProgram(Node *n, GLProgram *p)
{
    n->setGLProgram(p);
    if (n->getChildren().size() <= 0) return;
    Vector<Node *> children = n->getChildren();
    for (Vector<Node *>::iterator itor = children.begin(); itor != children.end(); ++itor) {
        Node *lNode = *itor;
        if (lNode) {
            this->setGLProgram(lNode, p);
        }
    }
}

vector<Vec2> SmearNode::interpolate(Vec2 pStartPosition, Vec2 pEndPosition, float pDelta)
{
    float lDistance = pStartPosition.getDistance(pEndPosition);
    int lDis = (int)lDistance;
    
    vector<Vec2> lPositions;
    for (int i = 0; i < lDis; i += pDelta) {
        float lDelta = float(i) / lDistance;
        float lDifX = pEndPosition.x - pStartPosition.x;
        float lDifY = pEndPosition.y - pStartPosition.y;
        
        Vec2 lPosition(pStartPosition.x + (lDifX * lDelta), pStartPosition.y + (lDifY * lDelta));
        lPositions.push_back(lPosition);
    }
    return lPositions;
}
