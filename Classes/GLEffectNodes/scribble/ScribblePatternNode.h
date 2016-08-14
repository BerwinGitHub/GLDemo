//
//  ScribblePatternNode.h
//  ScribbleDemo_3_2
//
//  Created by renhong on 9/26/14.
//
//

#ifndef __ScribbleDemo_3_2__ScribblePatternNode__
#define __ScribbleDemo_3_2__ScribblePatternNode__

#include <stdio.h>
#include "ScribbleNode.h"
#include "ScribblePattern.h"

class ScribblePatternNode : public ScribbleNode {
public:
    static ScribblePatternNode *create(Size pCanvasSize);
    ScribblePatternNode();
    virtual ~ScribblePatternNode();
    bool setPattern(string pShapeFile, Point pPosition);
    bool setPattern(string pShapeFile);
    void setTolerance(int pTolerance = 0);/*0~255*/
    bool useTarget(Color4F pTargetColor);
    bool useTarget(Sprite *pTarget);
    bool setPatternColor(Color4F pColor);
    inline void setDefaultColor(Color4F pColor){this->getCanvas()->clear(pColor.r, pColor.g, pColor.b, pColor.a);}
protected:
    ScribblePattern *getScribble(){return static_cast<ScribblePattern *>(_scribble);}
    virtual bool init(Size pCanvasSize);
    inline Image *getPatternImage(){return _patternImage;}
protected:
    Image *_patternImage;
};

#endif /* defined(__ScribbleDemo_3_2__ScribblePatternNode__) */
