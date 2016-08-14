//
//  ScribbleShapeNode.h
//  ScribbleDemo_3_2
//
//  Created by renhong on 9/26/14.
//
//

#ifndef __ScribbleDemo_3_2__ScribbleShapeNode__
#define __ScribbleDemo_3_2__ScribbleShapeNode__

#include <stdio.h>
#include "ScribbleNode.h"
#include "ScribbleShape.h"

class ScribbleShapeNode : public ScribbleNode {
public:
    static ScribbleShapeNode *create(Size pCanvasSize);
    ScribbleShapeNode();
    bool setShape(string pShapeFile, Point pPosition);
    bool setShape(string pShapeFile);
    bool useTargetColor(Color4F pTargetColor);
protected:
    ScribbleShape *getScribble(){return static_cast<ScribbleShape *>(_scribble);}
    virtual bool init(Size pCanvasSize);
};

#endif /* defined(__ScribbleDemo_3_2__ScribbleShapeNode__) */
