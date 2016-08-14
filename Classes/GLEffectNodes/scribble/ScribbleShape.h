//
//  ScribbleShape.h
//  ScribbleDemo_3_2
//
//  Created by renhong on 9/26/14.
//
//

#ifndef __ScribbleDemo_3_2__ScribbleShape__
#define __ScribbleDemo_3_2__ScribbleShape__

#include <stdio.h>
#include "Scribble.h"

#define kScribbleShapeShaderName "kScribbleShapeShaderName"

class ScribbleShape : public Scribble {
public:
    ScribbleShape(Sprite *pBrush);
    void setBrushShader();
//    virtual void setBrushType(Scribble::BrushType pType);
    void setShape(Sprite *pShape);
    void setShapeTexture(Sprite *pShape);
    void setTarget(Sprite *pTarget);
    void setTarget(Color4F pColor);
    inline  virtual Sprite *getTarget(){return _shape;}
protected:
    Sprite *_shape;
};

#endif /* defined(__ScribbleDemo_3_2__ScribbleShape__) */
