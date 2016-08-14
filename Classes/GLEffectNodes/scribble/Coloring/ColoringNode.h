//
//  ColoringNode.h
//  DrawDemo
//
//  Created by renhong on 9/15/14.
//
//

#ifndef __ColoringNode__
#define __ColoringNode__

#include <stdio.h>
#include <cocos2d.h>
#include <string>
using namespace std;
using namespace cocos2d;

extern CC_DLL const GLchar * Coloring_frag;

class ColoringNode : public Node {
public:
    struct ColoringArea{
        int left;
        int right;
        int top;
        int bottom;
        
        ColoringArea(int pLeft = 0, int pRight = 0, int pTop = 0, int pBottom = 0) : left(pLeft), right(pRight), top(pTop), bottom(pBottom){}
    };
    
    enum class ColoringMode{
        eFlat = 0x0,//平板着色
        eHorizonSmooth,//水平平滑着色（从左到右）
        eVerticalSmooth,//竖直平滑着色（从下到上）
        eDiagonalSmooth//对角平滑着色（从左下到右上）
    };
    
    enum class ColoringZOrder{
        eZCanvas,
        eZPattern,
        eZLineFrame
    };
    
public:
    static ColoringNode *create(string pTemplateFile, string pLineArtFile);
    ColoringNode();
    ~ColoringNode();
    bool init(string pTemplateFile, string pLineArtFile);
    virtual void setBrushShader();
    virtual bool coloring(Point pWorldPosition);
    Texture2D *getShapeBounds(Image *pShapeImage);
    
    inline void setColoringMode(ColoringMode pMode){_coloringMode = pMode;}
    inline void setStartColor(const Color4B &pColor){_startColor = pColor;}
    inline void setEndColor(const Color4B &pColor){_endColor = pColor;}
    inline void setDefaultColor(Color4F pColor = Color4F::WHITE){
        _canvasDefaultColor = pColor;
        _canvas->clear(pColor.r, pColor.g, pColor.b, pColor.a);
    }
    inline void resetPatternColor(){
        _prePatternColor = Color4B::BLACK;
    }
    void reset();//重置画布
    virtual void uploadRenderPara();
protected:
    virtual void initCanvas(Size pCanvasSize);
    virtual void initRenderPara();
    virtual void initPattern(string pPatternImage);
    bool touchedInsideLineframe(Vec2 pWorldPosition);
public:
    static const int gPixelSize = 5;
    static const int gTextureSize = 256;
    CC_SYNTHESIZE(bool, ignoreOutOfLineframe, IgnoreOutOfLineframe);//忽略线框图外部区域，该区域填色无效
protected:
    CC_SYNTHESIZE_RETAIN(Sprite *, _template, Template);
    Sprite *_lineArt;
    RenderTexture *_canvas;
    ColoringMode _coloringMode;
    Color4B _startColor;
    Color4B _endColor;
    
    Texture2D *_colorTexture;
    Texture2D *_endColorTexture;
    Texture2D *_colorModelTexture;
    Texture2D *_shapeBoundTexture;
    Image *_templateImage;
    Color4F _canvasDefaultColor;
    Color4B _prePatternColor;
};

#endif /* defined(__ColoringNode__) */