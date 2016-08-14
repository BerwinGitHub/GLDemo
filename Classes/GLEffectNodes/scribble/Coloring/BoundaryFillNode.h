#ifndef __BoundaryFillNode__
#define __BoundaryFillNode__

#include <stdio.h>
#include <string>
#include <cocos2d.h>
#include "ColorFillNode.h"
using namespace cocos2d;
using namespace std;

class BoundaryFillNode : public Node {
public:
    struct DIRECTION{
        int x_offset;
        int y_offset;
    };
public:
    BoundaryFillNode();
    ~BoundaryFillNode();
    static BoundaryFillNode *create(string pLineFrameFile);
    virtual bool init(string pLineFrameFile);
    
    Color4B getPixelColor(const int &pX, const int &pY);
    inline void setPixelColor(const int &pX, const int &pY, const Color4B &pColor);
    inline bool checkBounds(const int &pX, const int &pY);
    inline bool checkBounds(const int &pX, const int &pY, const Color4B &pLineBoundsColor);
    inline bool checkBounds(const Color4B &pColor, const Color4B &pBoundaryColor);
    void boundarySeedFill(const int &x, const int &y, const Color4B &new_color, const Color4B &boundary_color);
    
    void ScanLineSeedFill(const int &x, const int &y, const Color4B &new_color, const Color4B &boundary_color);
    void SearchLineNewSeed(std::stack<Point>& stk, const int &xLeft, const int &xRight, const int &y, const Color4B &new_color, const Color4B &boundary_color);
    int fillLineRight(const int &pX, const int &pY, const Color4B &pNewColor, const Color4B &pBoundaryColor);
    int fillLineLeft(const int &pX, const int &pY, const Color4B &pNewColor, const Color4B &pBoundaryColor);
    inline bool IsPixelValid(const int &pXt, const int &pY, const Color4B &pNewColor, const Color4B &pBoundaryColor);
    inline int SkipInvalidInLine(const int &pXt, const int &pY, const int &pRight, const Color4B &pNewColor, const Color4B &pBoundaryColor);
    void fill(Color4B pColor, Vec2 pWorldPositon);
    Size getSize(){return _lineArtSprite->getContentSize();}
    inline Sprite *getLineArt(){return _lineArtSprite;}
    void updateImage();
    void setBoundaryTolerance(int pTolerance = 0){_boundaryTolerance = pTolerance;}/*0~255*/
protected:
    static DIRECTION direction_8[];
    Sprite *_lineArtSprite;
    unsigned char *_imageData;
    int _boundaryTolerance;
};

#endif /* defined(__BoundaryFillNode__) */
