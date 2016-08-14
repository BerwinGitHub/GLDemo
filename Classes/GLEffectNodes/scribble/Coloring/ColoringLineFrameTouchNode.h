//
//  ColoringTouchNode.h
//  DrawDemo
//
//  Created by renhong on 9/15/14.
//
//

#ifndef __DrawDemo__ColoringLineFrameTouchNode__
#define __DrawDemo__ColoringLineFrameTouchNode__

#include <stdio.h>
#include "ColoringLineFrameNode.h"

class ColoringLineFrameTouchNode : public ColoringLineFrameNode{
public:
    static ColoringLineFrameTouchNode *create(string pLineArtFile);
    bool init(string pLineArtFile);
};
#endif /* defined(__DrawDemo__ColoringTouchNode__) */
