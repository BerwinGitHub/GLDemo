//
//  SmearNode.cpp
//  GLDemo
//
//  Created by tangbowen on 16/7/28.
//
//

#include "SmearNode.hpp"
#include "./shader/SmearShaderFrag.frag"
#include "./support/GLProgramUtility.hpp"
#include "./support/smearMath.hpp"

SmearNode::SmearNode()
: _pPaint(nullptr)
, _pTarget(nullptr)
, _fPaintHardness(1.0f)
, _sPaintMiddle(Vec2::ZERO)
, _sTargetMiddle(Vec2::ZERO)
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

SmearNode* SmearNode::create(const string &tFile, const string &pFile)
{
    return SmearNode::create(Target::create(tFile), Paint::create(pFile));
}

bool SmearNode::init(const Target *t, const Paint *p)
{
    if (!RenderTexture::initWithWidthAndHeight(t->getContentSize().width, t->getContentSize().height, Texture2D::PixelFormat::RGBA8888, 0)) {
        return false;
    }
    this->initShader();
    this->setPaintTexture(p->getTexture());
    this->setPaintType(SmearNode::PaintType::kPaint);
    
    this->setTargetTexture(t->getTexture());
    
    return true;
}

void SmearNode::draw(const cocos2d::Vec2 &pos)
{
    this->SmearNode::draw(vector<Vec2>{pos});
}

void SmearNode::draw(const cocos2d::Vec2 &startPos, const cocos2d::Vec2 &endPos)
{
    int delta = _pPaint->getContentSize().width * 0.25f * _pPaint->getScaleX();//5.0 * pBrush->getScale();  5.0 * pBrush->getScale();//
    vector<Vec2> vPosition = smearMath::interpolate(startPos, endPos, delta);
    this->SmearNode::draw(vPosition);
}

void SmearNode::draw(vector<cocos2d::Vec2> pWorldPositions)
{
    for (int i = 0; i < pWorldPositions.size(); ++i) {
        Vec2 pos = this->getSprite()->convertToNodeSpace(pWorldPositions.at(i));
        Size size = this->getSprite()->getContentSize();
        Rect boundingBox = Rect(0, 0, size.width, size.height);
        if (boundingBox.containsPoint(pos)) {
            //set the coord of pen at the target
            this->bindPaintPosition(pos);
            //draw
            _pPaint->setPosition(pos);
            
            this->begin();
            _pPaint->visit();
            this->end();
            Director::getInstance()->getRenderer()->render();
        }
    }
}

void SmearNode::drawSelf()
{
    Texture2D *texture = _pPaint->getTexture();
    float hard = this->getPaintHardness();
    SmearNode::PaintType eType = this->getPaintType();
    
    this->setPaintTexture(_pTarget->getTexture());
    this->setPaintHardness(1.0f);
    this->setPaintType(SmearNode::PaintType::kPaint);
    
    this->SmearNode::draw(this->getSprite()->convertToWorldSpace(_sTargetMiddle));
    
    this->setPaintTexture(texture);
    this->setPaintHardness(hard);
    this->setPaintType(eType);
}

void SmearNode::clearSelf()
{
    this->clear(0, 0, 0, 0);
}

void SmearNode::setPaintHardness(float hard)
{
    CCASSERT(hard <= 1.0f && hard >= 0.0f, "Hard value must in [0.0,1.0]");
    _fPaintHardness = hard;
    int solid = (hard == 1.0f) ? 1 : 0;
    solid = (_ePaintType == SmearNode::PaintType::kPaint) ? 1 : solid;
    GLUtility::bindUniformInt(_pPaint, "b_solid_enable", solid);
    GLUtility::bindUniformFloat(_pPaint, "f_solid_rate", 1.0f - hard);
}

float SmearNode::getPaintHardness()
{
    return _fPaintHardness;
}

void SmearNode::setPaintType(SmearNode::PaintType t)
{
    _ePaintType = t;
    switch (_ePaintType) {
        case SmearNode::PaintType::kPaint:{
            BlendFunc f = _bAntiAliasingPaint ? BlendFunc({GL_ONE, GL_ONE_MINUS_SRC_ALPHA }) : BlendFunc({GL_ONE, GL_ZERO});
            _pPaint->setBlendFunc(f);
            GLUtility::bindUniformInt(_pPaint, "b_solid_enable", 1);
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
        case SmearNode::PaintType::kEaser:{
            GLUtility::bindUniformInt(_pPaint, "b_solid_enable", 0);
            BlendFunc f = { GL_ZERO, GL_SRC_ALPHA };
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
    if (!tex)
        return;
    SpriteFrame *frame = SpriteFrame::createWithTexture(tex, Rect(0, 0, tex->getContentSize().width, tex->getContentSize().height));
    _pPaint->setSpriteFrame(frame);
    _sPaintMiddle = _pPaint->getContentSize() * 0.5f;
    this->bindPaintTexture();
}

void SmearNode::setTargetTexture(cocos2d::Texture2D *tex)
{
    if (!tex)
        return;
    Size ts = tex->getContentSize();
    _pTarget->setPosition(this->getSprite()->getContentSize() * 0.5f);
    SpriteFrame *frame = SpriteFrame::createWithTexture(tex, Rect(0, 0, ts.width, ts.height));
    _pTarget->setSpriteFrame(frame);
    _sTargetMiddle = _pTarget->getContentSize() * 0.5f;
    this->bindTargetTexture();
}

Target *SmearNode::getTarget()
{
    return _pTarget;
}

Paint *SmearNode::getPaint()
{
    return _pPaint;
}

void SmearNode::initShader()
{
    string key = GLUtility::generatedUniqueGLProgramKey(kSmearShaderName, this);
    GLProgram *p = GLUtility::loadGLProgram(ccPositionTextureColor_noMVP_vert, smearPositonTextureColor_frag, key);
    if (p && _pPaint->getGLProgram() != p) {
        this->setGLProgram(_pPaint, p);
        
        this->bindPaintAlphaValue(0.0f);
        this->bindTargetAlphaValue(0.0f);
        this->setPaintHardness(1.0f);
        this->bindPaintTexture();
    }
}

void SmearNode::bindPaintPosition(cocos2d::Vec2 p)
{
    float lX = (p.x - _pTarget->getPosition().x + _sTargetMiddle.width - _sPaintMiddle.width);
    float lY = _pTarget->getContentSize().height - (p.y - _pTarget->getPosition().y + _sTargetMiddle.height + _sPaintMiddle.height);
    GLUtility::bindUniformVec2(_pPaint, "v_texCoord_target", Vec2(lX, lY));
}

void SmearNode::bindPaintAlphaValue(float v)
{
    CCASSERT(v <= 1.0f && v >= 0.0f, "Apha value must in [0.0,1.0]");
    GLUtility::bindUniformFloat(_pPaint, "f_alpha_value_paint", v);
}

void SmearNode::bindTargetAlphaValue(float v)
{
    CCASSERT(v <= 1.0f && v >= 0.0f, "Apha value must in [0.0,1.0]");
    GLUtility::bindUniformFloat(_pPaint, "f_alpha_value_target", v);
}

void SmearNode::bindAntiAliasingPaint(bool b)
{
    _bAntiAliasingPaint = b;
    GLUtility::bindUniformInt(_pPaint, "b_paint_anti_aliasing", _bAntiAliasingPaint ? 1 : 0);
}

void SmearNode::bindPaintTexture()
{
    GLUtility::bindUniformVec2(_pPaint, "v_texSize_paint", _pPaint->getContentSize());
    GLUtility::bindUniformTexture(_pPaint, "s_texture_paint", _pPaint->getTexture());
}

void SmearNode::bindTargetTexture()
{
    GLUtility::bindUniformVec2(_pPaint, "v_texSize_target", _pTarget->getContentSize());
    GLUtility::bindUniformTexture(_pPaint, "s_texture_target", _pTarget->getTexture());
}

void SmearNode::onEnter()
{
    RenderTexture::onEnter();
}

void SmearNode::onExit()
{
    RenderTexture::onExit();
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
