//
//  TestBaseLayer.hpp
//  GLDemo
//
//  Created by tangbowen on 11/3/16.
//
//

#ifndef TestBaseScene_hpp
#define TestBaseScene_hpp

#include "cocos2d.h"
#include "ControllerScene.hpp"

USING_NS_CC;
using namespace std;

class TestBaseScene : public Layer{
    
public:
    TestBaseScene();
    virtual ~TestBaseScene();
    
public:
    virtual bool init() override;
    
protected:
    virtual string title(){ return ""; };
    
    void menuCallBack(Ref *ref);
    
protected:
    virtual void initReturnMenu();
    virtual void initTitle();
    
public:
    virtual void onEnter() override;
    virtual void onExit() override;
    
protected:
    Size    _visibleSize;
    
};

#endif /* TestBaseScene_hpp */
