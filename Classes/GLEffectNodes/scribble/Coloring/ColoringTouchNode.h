//
//  ColoringTouchNode.h
//  DrawDemo
//
//  Created by renhong on 9/15/14.
//
//

#ifndef __DrawDemo__ColoringTouchNode__
#define __DrawDemo__ColoringTouchNode__

#include <stdio.h>
#include "ColoringNode.h"

class ColoringTouchNode : public ColoringNode{
public:
    static ColoringTouchNode *create(string pTemplateFile, string pLineArtFile);
    bool init(string pTemplateFile, string pLineArtFile);
};
#endif /* defined(__DrawDemo__ColoringTouchNode__) */
