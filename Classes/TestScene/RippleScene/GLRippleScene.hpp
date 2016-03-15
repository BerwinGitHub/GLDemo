//
//  GLHSVScene.hpp
//  GLDemo
//
//  Created by tangbowen on 11/3/16.
//
//

#ifndef GLRippleScene_hpp
#define GLRippleScene_hpp

#include "cocos2d.h"
#include "HSLSprite.h"
#include "cocos-ext.h"
#include "TestBaseScene.hpp"

using namespace cocos2d::extension;
USING_NS_CC;

class GLRippleScene : public TestBaseScene
{
public:
    GLRippleScene();
    static Scene *scene();
    virtual bool init() override;
    virtual string title() override { return "RippleScene"; } ;
    
    CREATE_FUNC(GLRippleScene);
    
};

#endif /* GLRippleScene_hpp */
