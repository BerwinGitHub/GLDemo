//
//  GLProgramUtility.hpp
//  GameFrame
//
//  Created by 唐博文 on 16/8/6.
//
//

#ifndef GLProgramUtility_hpp
#define GLProgramUtility_hpp

#include <stdio.h>
#include "Cocos2d.h"

using namespace std;
using namespace cocos2d;

#define     GLUtility       GLProgramUtility

class GLProgramUtility {
    
#pragma mark --shader create & init
public:
    /**
     * @brief generate a unique glprogram key.
     *
     */
    static std::string generatedUniqueGLProgramKey(const std::string &name, void *p);
    
    /**
     * @brief load glprogram by key (FBO & VERT)
     */
    static GLProgram *loadGLProgram(const GLchar *vert, const GLchar *frag, const string &key);
    
    /**
     * @brief get glprogramstate if exist. otherwise create it
     */
    static GLProgramState *getGLProgramState(Node *n);
    
#pragma mark --shader value opr
public:
    /**
     * @brief bind shader value for type int
     */
    static void bindUniformInt(Node *n, const string &glAttr, int i);
    
    /**
     * @brief bind shader value for type float
     */
    static void bindUniformFloat(Node *n, const string &glAttr, float f);
    
    /**
     * @brief bind shader value for type vec2
     */
    static void bindUniformVec2(Node *n, const string &glAttr, const Vec2 &v);
    
    /**
     * @brief bind shader value for type texture
     */
    static void bindUniformTexture(Node *n, const string &glAttr, Texture2D *t);
};

#endif /* GLProgramUtility_hpp */
