//
//  SmearScene.hpp
//  GLDemo
//
//  Created by tangbowen on 11/3/16.
//
//

#ifndef SmearScene_hpp
#define SmearScene_hpp

#include "cocos2d.h"
#include "HSLSprite.h"
#include "TestBaseScene.hpp"

USING_NS_CC;

class SmearScene : public TestBaseScene
{
public:
    SmearScene();
    static Scene *scene();
    virtual bool init() override;
    virtual string title() override { return "SmearScene"; } ;
    
    CREATE_FUNC(SmearScene);
    
};

#endif /* SmearScene_hpp */
