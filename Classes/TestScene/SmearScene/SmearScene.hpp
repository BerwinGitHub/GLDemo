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
    ~SmearScene();
    static Scene *scene();
    virtual bool init() override;
    virtual string title() override { return "SmearScene"; } ;
    
    CREATE_FUNC(SmearScene);
    
public:
    void testSmearSegment(const Size &s);
    void testSmearRGBA(const Size &s);
    void testSmearArmature(const Size &s);
    void testSmearNode(const Size &s);
    
};

#endif /* SmearScene_hpp */
