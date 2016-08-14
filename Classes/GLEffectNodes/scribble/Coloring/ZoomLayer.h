//
//  ZoomLayer.h
//  ColourFillingTest
//
//  Created by renhong on 14-11-6.
//
//http://cn.cocos2d-x.org/tutorial/show?id=1479

#ifndef __ColourFilling__ZoomLayer__
#define __ColourFilling__ZoomLayer__

#include <iostream>
#include <cocos2d.h>
using namespace cocos2d;
class ZoomLayer : public LayerColor{
public:
    ~ZoomLayer();
    static ZoomLayer *create(Node *pNode);
    virtual bool init(Node *pNode);
private:
    Node *_contentNode;
    Point mpOrigin;
    EventListener *_touchEventListener;
};

#endif /* defined(__ColourFilling__ZoomLayer__) */
