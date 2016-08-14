//
//  ColorFillNode.h
//  ColourFillingTest
//
//  Created by renhong on 14-11-5.
//
//

#ifndef __ColourFilling__ColorFillNode__
#define __ColourFilling__ColorFillNode__

#include <iostream>
#include <cocos2d.h>
using namespace cocos2d;

class ColorFillNode;
class ColorFillNodeDelegate {
public:
    virtual void willColoring(ColorFillNode *pNode) = 0;
};

class ColorFillNode : public Node {
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
    
public:
    ColorFillNode();
    ~ColorFillNode();
    virtual inline void setColoringMode(ColoringMode pMode){_coloringMode = pMode;}
    virtual inline void setStartColor(const Color4B &pColor){_startColor = pColor;}
    inline Color4B getStartColor(){return _startColor;}
    virtual inline void setEndColor(const Color4B &pColor){_endColor = pColor;}
    inline Color4B getEndColor(){return _endColor;}
    inline cocos2d::Size getSize() const{return _lineArtSprite->getContentSize();}
    inline Sprite *getLineArt(){return _lineArtSprite;}
    inline void setDelegate(ColorFillNodeDelegate *pDelegate){_delegate = pDelegate;}
    
protected:
    ColoringMode _coloringMode;
    Color4B _startColor;
    Color4B _endColor;
    ColorFillNodeDelegate *_delegate;
    Sprite *_lineArtSprite;
    EventListener *_touchEventListener;
};

#endif /* defined(__ColourFilling__ColorFillNode__) */
