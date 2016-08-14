//
//  ScribbleLineFrameNode.h
//  ScribbleDemo_3_2
//
//  Created by renhong on 8/14/15.
//
//

#ifndef __ScribbleDemo_3_2__ScribbleLineFrameNode__
#define __ScribbleDemo_3_2__ScribbleLineFrameNode__

#include <stdio.h>
#include "ScribblePatternNode.h"
#include "./Coloring/Pattern.h"

class ScribbleLineFrameNode;
class ScribbleLineFrameLoadDelegate{
public:
    virtual void loadPercent(Node *pNode, float pPercent) = 0;//Pattern加载进度0.0~1.0
    virtual void loadEnded(Node *pNode) = 0;
};

/*直接使用线框图进行边界判断与涂抹*/
class ScribbleLineFrameNode : public ScribblePatternNode, public PatternLoadDelegate {
public:
    static ScribbleLineFrameNode *create(Size pCanvasSize);
    ScribbleLineFrameNode();
    ~ScribbleLineFrameNode();
    bool setPattern(string pLineFrameFile, bool pAutoFill = false, int pTolerance = 0);
    void paint(Point pPosition);
    void paint(Point pStartPosition, Point pEndPosition);
    void setDelegate(ScribbleLineFrameLoadDelegate *pDelegate){_delegate = pDelegate;}
    ScribbleLineFrameLoadDelegate *getDelegate(){return _delegate;}
    void setLineFrame(Node *pLineFrame, Vec2 pPosition = Vec2::ZERO);
    void debugPattern(bool pDebug = true);
protected:
    virtual void loadPercent(Pattern *pNode, float pPercent);
    virtual void loadEnded(Pattern *pNode);
    CC_SYNTHESIZE_RETAIN(Pattern *, _patternNode, PatternNode);
private:
    Node *_lineFrameNode;
    ScribbleLineFrameLoadDelegate *_delegate;
    bool _debugPattern;
};

#endif /* defined(__ScribbleDemo_3_2__ScribbleLineFrameNode__) */
