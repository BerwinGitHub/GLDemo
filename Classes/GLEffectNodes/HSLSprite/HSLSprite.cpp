//
//  HSLSprite.cpp
//  Demo
//
//  Created by tangbowen on 10/3/16.
//
//

#include "HSLSprite.h"
#include "./shader/HSLShader.frag"

HSLSprite::HSLSprite()
{
    
}

HSLSprite::~HSLSprite()
{
}

HSLSprite* HSLSprite::createWithHSL(const string &file, HSLSprite::HSL hsl)
{
    HSLSprite *pRet = new HSLSprite();
    if (pRet && pRet->initWithHSL(file, hsl)) {
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_RELEASE_NULL(pRet);
    return nullptr;
}

bool HSLSprite::initWithHSL(const string &file, HSLSprite::HSL hsl)
{
    if (!Sprite::initWithFile(file)) {
        return false;
    }
    this->setHSL(hsl.h, hsl.s, hsl.l);
    this->initShaderProgram();
   
    return true;
}

void HSLSprite::setHSL(float h, float s, float l)
{
    CCASSERT((h >= -180.0f && h <= 180.0f), "Hue Range--------->(-180.0f ~ 180.0f)");
    CCASSERT((s >= -1.0f && s <= 1.0f),     "Stauration Range-->(-1.0f ~ 1.0f)");
    CCASSERT((l >= -1.0f && l <= 1.0f),     "Lightness Range--->(-1.0f ~ 1.0f)");
    HSL hsl = HSL{h, s, l};
    this->setHSL(hsl);
}

void HSLSprite::setHue(float h)
{
    this->setHSL(h, _sHsl.s, _sHsl.l);
}

void HSLSprite::setStauration(float s)
{
    this->setHSL(_sHsl.h, s, _sHsl.l);
}

void HSLSprite::setLightness(float l)
{
    this->setHSL(_sHsl.h, _sHsl.s, l);
}

void HSLSprite::initShaderProgram()
{
    //blendfunc
    BlendFunc blendFunc={GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA};
    this->setBlendFunc(blendFunc);
    
    //create and set shader program
    {
        GLProgramUnifos* program = new GLProgramUnifos();
        program->autorelease();
        program->initWithByteArrays(ccPositionTextureColor_noMVP_vert, HSL_Shader_C_STR);
        program->link();
        //get cocos2d-x build-in uniforms
        program->updateUniforms();
        //get my own uniforms
        program->attachUniform("u_dH");
        program->attachUniform("u_dS");
        program->attachUniform("u_dL");
        //set program
        _pAdjustHSLUnifos = program;
        _pAdjustHSLUnifos->retain();
        //check gl error
        CHECK_GL_ERROR_DEBUG();
    }
    //check gl error
    CHECK_GL_ERROR_DEBUG();
}

void HSLSprite::draw(cocos2d::Renderer *renderer, const cocos2d::Mat4 &transform, uint32_t flags)
{
    //----change shader
    //pass values for cocos2d-x build-in uniforms
    this->setGLProgram(_pAdjustHSLUnifos);
    GLProgramUnifos *program= (GLProgramUnifos*)this->getGLProgram();
    program->use();
    program->setUniformsForBuiltins();
    //pass values for my own uniforms
    program->passUnifoValue1f("u_dH", _sHsl.h);
    program->passUnifoValue1f("u_dS", _sHsl.s);
    program->passUnifoValue1f("u_dL", _sHsl.l);
    
    //draw quad
    {
        _quadCommand.init(_globalZOrder, _texture->getName(), getGLProgramState(), _blendFunc, &_quad, 1, transform);
        renderer->addCommand(&_quadCommand);
    }
}

void HSLSprite::onEnter()
{
    Sprite::onEnter();
}

void HSLSprite::onExit()
{
    Sprite::onExit();
}