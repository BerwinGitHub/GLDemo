//
//  BlurScene.hpp
//  GLDemo
//
//  Created by tangbowen on 11/3/16.
//
//

#ifndef BlurScene_hpp
#define BlurScene_hpp

#include "cocos2d.h"
#include "HSLSprite.h"
#include "cocos-ext.h"
#include "TestBaseScene.hpp"

using namespace cocos2d::extension;
USING_NS_CC;

class BlurScene : public TestBaseScene
{
public:
    BlurScene();
    static Scene *scene();
    virtual bool init() override;
    virtual string title() override { return "BlurScene"; } ;
    
    CREATE_FUNC(BlurScene);
    
};

#endif /* BlurScene_hpp */
