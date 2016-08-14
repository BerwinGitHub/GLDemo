#include "BoundaryFillNode.h"
#include <string>
#include <map>
#include "ScribbleUtil.h"
using namespace std;
BoundaryFillNode::DIRECTION BoundaryFillNode::direction_8[] = { {-1, 0}, {-1, 1}, {0, 1}, {1, 1}, {1, 0}, {1, -1}, {0, -1}, {-1, -1} };
const float gMaxAlpha = 256;//200;//
const float gMinAlpha = 0;

BoundaryFillNode *BoundaryFillNode::create(string pLineFrameFile){
    BoundaryFillNode *ret = new BoundaryFillNode();
    if(ret && ret->init(pLineFrameFile)){
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

BoundaryFillNode::BoundaryFillNode(){
    _imageData = nullptr;
    _boundaryTolerance = 0;
}

BoundaryFillNode::~BoundaryFillNode(){
    delete [] _imageData;
}

Color4B BoundaryFillNode::getPixelColor(const int &pX, const int &pY){
    /*
    unsigned char *data = _imageData;//_lineImage->getData();
    
    const int bytesPerPixel = 4;//_lineImage->getBitPerPixel() / 8;
    
    int index = pY * _imageWidth + pX;//_lineImage->getWidth()
    index *= bytesPerPixel;
    unsigned char r = data[index];
    unsigned char g = data[index + 1];
    unsigned char b = data[index + 2];
    unsigned char a = data[index + 3];
    
//    CCLOG("getPixelColor %d %d %d %d at %d %d", r, g, b, a , pX, pY);
    return Color4B(r, g, b, a);
     */
    unsigned int *data = (unsigned int *)(_imageData);
    data += (pY * (int)this->getSize().width + pX);
    Color4B lColor;
    lColor.r = *data & 0xFF;
    lColor.g = (*data >> 8) & 0xFF;
    lColor.b = (*data >> 16) & 0xFF;
    lColor.a = (*data >> 24) & 0xFF;
    return lColor;
    
}

void BoundaryFillNode::setPixelColor(const int &pX, const int &pY, const Color4B &pColor){
    /*
    unsigned char *data = _imageData;//_lineImage->getData();
    const int bytesPerPixel = 4;//_lineImage->getBitPerPixel() / 8;
    int index = pY * _imageWidth + pX;//_lineImage->getWidth()
    index *= bytesPerPixel;
    data[index] = pColor.r;
    data[index + 1] = pColor.g;
    data[index + 2] = pColor.b;
    data[index + 3] = pColor.a;
    */
    unsigned int *data = (unsigned int *)(_imageData);
    data += (pY * (int)this->getSize().width + pX);
    *data = (pColor.r | (pColor.g << 8) | (pColor.b << 16) | (pColor.a << 24));
}

bool BoundaryFillNode::checkBounds(const int &pX, const int &pY){
    Rect lBounds(0, 0, (int)this->getSize().width - 1, (int)this->getSize().height - 1);
    return lBounds.containsPoint(Point(pX, pY));
}

bool BoundaryFillNode::checkBounds(const int &pX, const int &pY, const Color4B &pBoundaryColor){
    return this->checkBounds(getPixelColor(pX, pY), pBoundaryColor);
}

bool BoundaryFillNode::checkBounds(const Color4B &pColor, const Color4B &pBoundaryColor){
    bool lPass = false;
    //灰色/黑色则测试失败，不变色
//    bool lIsBigGray = (pColor.r - pBoundaryColor.r < _boundaryTolerance) && (pColor.r == pColor.g) && (pColor.r == pColor.b);
    //    bool lIsBlack = (curColor.r == 0) && (curColor.r == curColor.g) && (curColor.r == curColor.b);
//    lPass = !(lIsBigGray/* || lIsBlack*/);
    if (Color3B(pColor) == Color3B(pBoundaryColor) && abs(pColor.a - pBoundaryColor.a) <= _boundaryTolerance) {//边界
        lPass = false;
    }
    else{
        lPass = true;
    }
    return lPass;
}

void BoundaryFillNode::boundarySeedFill(const int &x, const int &y, const Color4B &new_color, const Color4B &boundary_color){
    if (checkBounds(x, y)) {
        Color4B curColor = getPixelColor(x, y);
        if (this->checkBounds(curColor, boundary_color) && (curColor != new_color)){
            setPixelColor(x, y, new_color);
            for(int i = 0; i < 8; i++){
                boundarySeedFill(x + direction_8[i].x_offset, y + direction_8[i].y_offset, new_color,boundary_color);
            }
        }
    }
}

bool BoundaryFillNode::IsPixelValid(const int &pXt, const int &pY, const Color4B &pNewColor, const Color4B &pBoundaryColor){
    if (checkBounds(pXt, pY)) {
        Color4B curColor = getPixelColor(pXt, pY);
        if(this->checkBounds(curColor, pBoundaryColor)&& (curColor != pNewColor)){
            return true;
        }
    }
    
    return false;
}

int BoundaryFillNode::SkipInvalidInLine(const int &pXt, const int &pY, const int &pRight, const Color4B &pNewColor, const Color4B &pBoundaryColor){
    return 0;
}

void BoundaryFillNode::SearchLineNewSeed(std::stack<Point>& stk, const int &xLeft, const int &xRight,const int &y, const Color4B &new_color, const Color4B &boundary_color){
    int xt = xLeft;
    bool findNewSeed = false;
    
    while(xt <= xRight){
        findNewSeed = false;
        while(IsPixelValid(xt, y, new_color, boundary_color) && (xt < xRight)){
            findNewSeed = true;
            xt++;
        }
        if(findNewSeed){
            if(IsPixelValid(xt, y, new_color, boundary_color) && (xt == xRight))
                stk.push(Point(xt, y));
            else
                stk.push(Point(xt - 1, y));
        }
        
        /*向右跳过内部的无效点（处理区间右端有障碍点的情况）*/
        int xspan = SkipInvalidInLine(xt, y, xRight, new_color, boundary_color);
        xt += (xspan == 0) ? 1 : xspan;
        /*处理特殊情况,以退出while(x<=xright)循环*/
    }
}

void BoundaryFillNode::ScanLineSeedFill(const int &x, const int &y, const Color4B &new_color, const Color4B &boundary_color){
    map<int, bool> searched;
    std::stack<Point> stk;
    stk.push(Point(x, y)); //第1步，种子点入站
    while(!stk.empty()){
        Point seed = stk.top(); //第2步，取当前种子点
        stk.pop();
        //第3步，向左右填充
        int count = fillLineRight(seed.x, seed.y, new_color, boundary_color);//向'cf?右'd3?填'cc?充'b3?
        int xRight = seed.x + count - 1;
        count = fillLineLeft(seed.x - 1, seed.y, new_color, boundary_color);//向'cf?左'd7?填'cc?充'b3?
        int xLeft = seed.x - count;
//        log("setPixelColor %d %d %d %d %d %d", xLeft, xRight, new_color.r, new_color.g, new_color.b, new_color.a);
//        setPixelColor(xLeft, xRight, seed.y, new_color);
        //第4步，处理相邻两条扫描线
//        if (!searched.find(seed.y - 1)->second) {
//            searched.insert(make_pair(seed.y - 1, true));
            SearchLineNewSeed(stk, xLeft, xRight, seed.y - 1, new_color, boundary_color);
//            log("search line %d", (int)seed.y - 1);
//        }
        
//        if (!searched.find(seed.y + 1)->second) {
//            searched.insert(make_pair(seed.y + 1, true));
            SearchLineNewSeed(stk, xLeft, xRight, seed.y + 1, new_color, boundary_color);
//            log("search line %d", (int)seed.y + 1);
//        }
    }
}

int BoundaryFillNode::fillLineRight(const int &pX, const int &pY, const Color4B &pNewColor, const Color4B &pBoundaryColor){
    int lX = pX;
    int lY = pY;
    while (true) {
        if (checkBounds(lX, lY)) {
//            Color4B curColor = getPixelColor(lX, lY);
            if(this->checkBounds(lX, lY, pBoundaryColor)){
//                if (curColor != pNewColor) {
                    setPixelColor(lX, lY, pNewColor);
//                }
                lX += 1;
            }
            else{
                break;
            }
        }
        else{
            break;
        }
    }
//    setPixelColor(pX, lX, lY, pNewColor);
    return lX - pX;
}

int BoundaryFillNode::fillLineLeft(const int &pX, const int &pY, const Color4B &pNewColor, const Color4B &pBoundaryColor){
    int lX = pX;
    int lY = pY;
    while (true) {
        if (checkBounds(lX, lY)) {
            if(this->checkBounds(lX, lY, pBoundaryColor)){
                //                if (curColor != pNewColor) {
                setPixelColor(lX, lY, pNewColor);
                //                }
                lX -= 1;
            }
            else{
                break;
            }
        }
        else{
            break;
        }
    }
//    log("setPixelColor %d %d %d %d %d %d", lX, pX, pNewColor.r, pNewColor.g, pNewColor.b, pNewColor.a);
//    setPixelColor(lX, pX, lY, pNewColor);
    return pX - lX;
}

void BoundaryFillNode::fill(Color4B pColor, Vec2 pWorldPositon){
    Point pos = this->convertToNodeSpace(pWorldPositon);//pWorldPositon;//
    int row = this->getSize().height - pos.y;
    int col = pos.x;
    //CCLOG("Touch %d %d Color %d %d %d %d", col, row, _startColor.r, _startColor.g, _startColor.b, _startColor.a);

    //this->boundarySeedFill(col, row, pColor, Color4B(0, 0, 0, 255));
    this->ScanLineSeedFill(col, row, pColor, Color4B(0, 0, 0, 255));

//    _lineArtSprite->getTexture()->updateWithData(_imageData, 0, 0, (int)this->getSize().width, (int)this->getSize().height);
}

void BoundaryFillNode::updateImage(){
    _lineArtSprite->getTexture()->updateWithData(_imageData, 0, 0, (int)this->getSize().width, (int)this->getSize().height);
}

bool BoundaryFillNode::init(string pLineFrameFile){
    if (!Node::init()) {
        return false;
    }
    
    Image *lLineImage = new Image();
    lLineImage->retain();
    lLineImage->initWithImageFile(pLineFrameFile);
    
    _imageData = new unsigned char[lLineImage->getDataLen()];
    memcpy(_imageData, lLineImage->getData(), lLineImage->getDataLen());
    
    Texture2D *lColorTexture = new Texture2D;
    lColorTexture->initWithImage(lLineImage);
    Sprite *lLineSprite = Sprite::createWithTexture(lColorTexture);
    lColorTexture->release();
    lLineImage->release();
    this->addChild(lLineSprite);
//    lLineSprite->setVisible(false);
    this->setContentSize(lLineSprite->getContentSize());
    lLineSprite->setPosition(lLineSprite->getContentSize() / 2);
    _lineArtSprite = lLineSprite;
    log("Size %f %f %s", _lineArtSprite->getContentSize().width, _lineArtSprite->getContentSize().height, pLineFrameFile.c_str());
    return true;
}