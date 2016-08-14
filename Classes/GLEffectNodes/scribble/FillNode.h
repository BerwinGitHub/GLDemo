//
//  FillNode.hpp
//  ScribbleDemo_3_2
//
//  Created by renhong on 15/12/16.
//
//

#ifndef FillNode_hpp
#define FillNode_hpp

#include <stdio.h>
#include "cocos2d.h"
using namespace cocos2d;

#include "ScribbleLineFrameNode.h"
#include "ColoringLineFrameNode.h"

class FillNode : public ScribbleLineFrameNode{
public:
    static FillNode *create(Size pCanvasSize);
    FillNode();
    bool init(Size pCanvasSize);
    bool setPattern(string pLineFrameFile, bool pAutoFill = false, int pTolerance = 0);
    
    void loadEnded(Pattern *pNode);
private:
    CC_SYNTHESIZE_READONLY(ColoringLineFrameNode *, _coloringNode, ColoringNode)
    ;
};

#endif /* FillNode_hpp */
