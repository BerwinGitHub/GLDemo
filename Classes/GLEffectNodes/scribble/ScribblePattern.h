//
//  ScribblePattern.h
//  ScribbleDemo_3_2
//
//  Created by renhong on 9/26/14.
//
//

#ifndef __ScribbleDemo_3_2__ScribblePattern__
#define __ScribbleDemo_3_2__ScribblePattern__

#include <stdio.h>
#include "Scribble.h"

#define kScribblePatternShaderName "kScribblePatternShaderName"

class ScribblePattern : public Scribble {
public:
    ScribblePattern(Sprite *pBrush);
    virtual ~ScribblePattern();
    void setBrushShader();
//    virtual void setBrushType(Scribble::BrushType pType);
    void setPattern(Sprite *pPattern);
    void setPatternTexture(Sprite *pPattern);
    void setTolerance(int pTolerance = 0);/*0~255*/
    void setTarget(Sprite *pTarget);
    void setTarget(Color4F pColor);
    void setPatternColor(Color4F pColor);
    inline  virtual Sprite *getTarget(){return _pattern;}
protected:
    Sprite *_pattern;
};

#endif /* defined(__ScribbleDemo_3_2__ScribblePattern__) */
