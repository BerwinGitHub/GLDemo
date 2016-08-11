//
//  GLProgramUtility.cpp
//  GameFrame
//
//  Created by 唐博文 on 16/8/6.
//
//

#include "GLProgramUtility.hpp"

#pragma mark --shader create & init

string GLProgramUtility::generatedUniqueGLProgramKey(const string &name, void *p)
{
    char cKey[512];
    sprintf(cKey, "%s%p", name.c_str(), p);
    return string(cKey);
}

GLProgram *GLProgramUtility::loadGLProgram(const GLchar *vert, const GLchar *frag, const string &key)
{
    GLProgram *p = GLProgram::createWithByteArrays(vert, frag);
    CHECK_GL_ERROR_DEBUG();
    if (nullptr != p) {
        GLProgramCache::getInstance()->addGLProgram(p, key);
    }
    return p;
}

GLProgramState* GLProgramUtility::getGLProgramState(cocos2d::Node *n)
{
    GLProgram *p = n->getGLProgram();
    if (!p)
        return nullptr;
    GLProgramState *state = GLProgramState::getOrCreateWithGLProgram(p);
    return state;
}

#pragma mark --shader value opr

void GLProgramUtility::bindUniformInt(cocos2d::Node *n, const string &glAttr, int i)
{
    GLProgramState *state = GLProgramUtility::getGLProgramState(n);
    state->setUniformInt(glAttr, i);
}

void GLProgramUtility::bindUniformFloat(cocos2d::Node *n, const string &glAttr, float f)
{
    GLProgramState *state = GLProgramUtility::getGLProgramState(n);
    state->setUniformFloat(glAttr, f);
}

void GLProgramUtility::bindUniformVec2(cocos2d::Node *n, const string &glAttr, const cocos2d::Vec2 &v)
{
    GLProgramState *state = GLProgramUtility::getGLProgramState(n);
    state->setUniformVec2(glAttr, v);
}

void GLProgramUtility::bindUniformTexture(cocos2d::Node *n, const string &glAttr, cocos2d::Texture2D *t)
{
    GLProgramState *state = GLProgramUtility::getGLProgramState(n);
    state->setUniformTexture(glAttr, t);
}
