//
//  StackBlurSprite.h
//  JniHelper
//
//  Created by tangbowen on 27/11/15.
//
//

#ifndef StackBlurSprite_h
#define StackBlurSprite_h

#include "cocos2d.h"

USING_NS_CC;
using namespace std;

class StackBlurSprite : public Sprite{
    
public:
    StackBlurSprite();
    ~StackBlurSprite();
    
public:
    static StackBlurSprite* createWithRadius(const string &file, int radius);
    bool initWithRadius(const string &file, int radius);
    
public:
    void setBlurRadius(int radius);
    int getBlurRadius();
    
private:
    int     _iRadius;
    
    unsigned char*  _originalData;
    size_t          _originalDataLen;
    int             _pixelsWide;
    int             _pixelsHigh;
    
private:
    void initImageData();
    void changeRadiusTexture();
    
public:
    virtual void onEnter() override;
    virtual void onExit() override;
};

#endif /* StackBlurSprite_h */
