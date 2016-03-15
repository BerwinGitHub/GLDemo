//
//  ControllerScene.hpp
//  GLDemo
//
//  Created by tangbowen on 11/3/16.
//
//

#ifndef ControllerScene_hpp
#define ControllerScene_hpp

#include "cocos2d.h"

USING_NS_CC;
using namespace std;

class ControllerScene : public Layer{

public:
    ControllerScene();
    ~ControllerScene();

public:
    static Scene* scene();
    CREATE_FUNC(ControllerScene);
    
    virtual bool init() override;
    
public:
    virtual void onEnter() override;
    virtual void onExit() override;
    
private:
    void initCloseButton();
    void closeMenuCallBack(Ref *ref);
    
    void initList();
    void menuCallBack(Ref *ref);
    
    bool onTouchBegan(Touch* touches, Event  *event);
    void onTouchMoved(Touch* touches, Event  *event);
    
    void onMouseScroll(Event *event);
    
private:
    Size    _visibleSize;
    Menu    *_itemMenu;
    Vec2    _beginPos;
    
};


#endif /* ControllerScene_hpp */
