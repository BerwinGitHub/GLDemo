//
//  Scribble.h
//  GIRLS_Framework_part
//
//  Created by renhong on 9/12/13.
//
//

#ifndef __GIRLS_Framework_part__Scribble__
#define __GIRLS_Framework_part__Scribble__
#include <cocos2d.h>
#include <iostream>
#include <string>
using namespace cocos2d;
using namespace std;

extern CC_DLL const GLchar * ScribbleShader_frag;
#define kScribbleShaderName "kScribbleShaderName"

class Scribble {//Support cocos2d-x 3.2  and later
public:
    typedef enum{
        eBrush,
        eAdditiveBrush,
        eAdditiveBrush_Src,
        eEaser
    }BrushType;
private:
    Sprite *_brush;
    BrushType _brushType;
    Sprite *_target;
    bool _antiAliasingBrush;
protected:
    
public:
    Scribble(Sprite *pBrush);
    virtual ~Scribble();
    void antiAliasing(RenderTexture *pRenderTexture);
    void disAntiAliasing(RenderTexture *pRenderTexture);
    
    void paint(Sprite *pTarget, RenderTexture *pCanvas, Point pPosition);
    void paint(Sprite *pTarget, RenderTexture *pCanvas, Point pStartPosition, Point pEndPosition);
    void paint(Sprite *pTarget, RenderTexture *pCanvas, vector<Point> pWorldPositions);
    void setAntiAliasingBrush(bool pAntiAliasingBrush);//是否开启画笔抗锯齿。 注意：半透明的绘制对象暂时不支持画笔抗锯齿！
    void setBrush(Texture2D *pBrushTexture);
    
    virtual void setBrushShader();
    virtual void setBrushType(BrushType pType = eBrush);
    void setBrushAlphaTestValue(float pAlphaValue);
    
    virtual void setTarget(Sprite *pTarget);
    void setTargetSolid(bool pIsSolid, float pSolidRate = 1.0/*淡出效果，建议pSolidRate取值0.05，取值越大，擦除越快，1.0为即时擦除*/);////renhong, 重构时pIsSolid参数可以简化。
    void setTargetAlphaTestValue(float pAlphaValue);
    
    void removeTargetObserver(RenderTexture *pTarget);
    
    inline Sprite *getBrush(){return _brush;}
    inline  virtual Sprite *getTarget(){return _target;}
    
protected:
    inline void setPaintPosition(Sprite *pTarget, Sprite *pBrush, Point pWorldPosition);
    inline void setTargetTexture(Sprite *pTexture);
    inline void setBrushTexture();
};
#endif /* defined(__GIRLS_Framework_part__Scribble__) */
