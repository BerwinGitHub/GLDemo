//
//  ScribbleTouchNode.h
//  ScribbleDemo_3_2
//
//  Created by renhong on 9/12/14.
//
//

#ifndef __ScribbleTouchNode__
#define __ScribbleTouchNode__

#include <stdio.h>
#include "ScribbleNode.h"

class ScribbleTouchNode : public ScribbleNode {
public:
    enum class TouchType{
        eTouchBegan = 0x1,
        eTouchMoved = 0x2,
        eTouchEnded = 0x4
    };
    static ScribbleTouchNode *create(Size pCanvasSize);
    
    virtual bool touchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
    virtual void touchMoved(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
    virtual void touchEnded(cocos2d::Touch *pTouch, cocos2d::Event *pEvent);
    void setSwallowTouches(bool pSwallow){_touchListener->setSwallowTouches(pSwallow);}//本类对象是否吞噬点击消息
    void enable(){_enabled = true;}//点击事件的自动响应开启，本类对象会自动处理涂抹动作。
    void disable(){_enabled = false;}//点击事件的自动响应关闭，本类对象不处理涂抹动作，需要程序员自己处理，此时与ScribbleNode类完全相同。
    bool isEnabled(){return _enabled;}
    void enableTouchBegan(bool pEnable){_enableTouchBegan = pEnable;}//启(禁)用“TouchBegan”自动涂抹
    void enableTouchMoved(bool pEnable){_enableTouchMoved = pEnable;}//启(禁)用“TouchMoved”自动涂抹
    void enableTouchEnded(bool pEnable){_enableTouchEnded = pEnable;}//启(禁)用“TouchEnded”自动涂抹
    
protected:
    bool init(Size pCanvasSize);
    ScribbleTouchNode();
    ~ScribbleTouchNode();
    
protected:
    bool _enabled;
    bool _enableTouchBegan;
    bool _enableTouchMoved;
    bool _enableTouchEnded;
    EventListenerTouchOneByOne *_touchListener;
};

#endif /* defined(__ScribbleTouchNode__) */
