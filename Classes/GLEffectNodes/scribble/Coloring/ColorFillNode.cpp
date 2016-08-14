//
//  ColorFillNode.cpp
//  ColourFillingTest
//
//  Created by renhong on 14-11-5.
//
//

#include "ColorFillNode.h"

ColorFillNode::ColorFillNode(){
    _delegate = nullptr;
    _lineArtSprite = nullptr;
    _touchEventListener = nullptr;
}

ColorFillNode::~ColorFillNode(){
//    _lineArtSprite->release();
    this->getEventDispatcher()->removeEventListener(_touchEventListener);
}