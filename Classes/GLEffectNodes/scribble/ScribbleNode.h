//
//  ScribbleNode.h
//  ScribbleDemo_3_2
//
//  Created by renhong on 8/4/14.
//
//

#ifndef __ScribbleNode__
#define __ScribbleNode__

#include <iostream>
#include <string>
#include <cocos2d.h>
#include "Scribble.h"
using namespace std;
using namespace cocos2d;

typedef Sprite Brush;
typedef Sprite Target;
typedef RenderTexture Canvas;

class ScribbleNode : public Node {//Support cocos2d-x 3.2 and later
public:
#pragma mark - 常见运动轨迹插值
    inline static vector<Point> interpolate(Point pStartPosition, Point pEndPosition, float pDelta);//两点间线性插值
    inline static vector<Point> waterPositions(Point pFromPoint, float pMaxX, float pDelta, Vec2 pDirection);//线性插值,模拟冲水效果
#pragma mark - 构造与初始化
    static ScribbleNode *create(int pCanvasWidth, int pCanvasHeight){return create(Size(pCanvasWidth, pCanvasHeight));}
    static ScribbleNode *create(Size pCanvasSize);
    ScribbleNode();
    virtual ~ScribbleNode();
    
#pragma mark - 抗锯齿
    void antiAliasing();
    void disAntiAliasing();
    void setAntiAliasingBrush(bool pAntiAliasingBrush);
#pragma mark - 绘画
    bool useBrush(Scribble::BrushType pBrushType);//修改当前画刷类型：画笔/橡皮擦
    bool useBrush(string pBrushFile, Scribble::BrushType pBrushType = Scribble::eBrush, float pScale = 1.0);//设置画刷，并指定其类型：画笔/橡皮擦
    bool addTarget(string pTargetFile, Vec2 pPositionInCanvas);//添加绘制对象，并设置对象在画布中的位置。
    bool addTarget(string pTargetFile);//添加绘制对象，将绘制对象的位置默认设置为画布中央
    bool useTarget(string pTargetFile, Vec2 pPositionInCanvas);//启用addTarget设定的一个绘制的对象，并根据需要设置对象的绘制位置。
    bool useTarget(string pTargetFile);//启用addTarget设定的一个绘制的对象，并使用addTarget(string pTargetFile, Vec2 pPositionInCanvas)或addTarget(string pTargetFile)预设的绘制位置。
    bool useTarget(Sprite *pTarget, Vec2 pPositionInCanvas);
    bool useTarget(Sprite *pTarget);
    bool resetCanvas();
    bool setBrushSolid(bool pSolid = true, float pSolidRate = 1.0);
    void setBrushAlphaTestValue(float pAlphaValue = 0.0f);
    
#pragma mark - Get
    inline Scribble *getScribble(){return _scribble;}
    inline Canvas *getCanvas(){return _canvas;}
    inline Size getCanvasSize(){return _canvasSize;}

#pragma mark - paint
    /*
     //注意：
     //如果你对效率要求较高，请在外部调用interpolate或waterPositions之类的插值算法插值得到将要绘制的点，
     //然后传入重载函数void paint(vector<Point> pWorldPositions);
     */
    virtual void paint(string pPath);
    virtual void paint(Point pWorldPosition);
    virtual void paint(Point pStartWorldPosition, Point pEndWorldPosition);
    virtual void paint(vector<Point> pWorldPositions);
    
protected:
    virtual bool init(Size pCanvasSize);
    
protected:
    Brush *_brush;
    Map<string, Target *> _targets;
    Canvas *_canvas;
    Size _canvasSize;
    Scribble *_scribble;
};

#endif /* defined(__ScribbleNode__) */
