//
//  RippleSprite.hpp
//  Demo
//
//  Created by tangbowen on 10/3/16.
//
//

#ifndef RippleSprite_hpp
#define RippleSprite_hpp

#include "cocos2d.h"
#include "../support/Mesh.h"
#include "../support/IndexVBO.h"

USING_NS_CC;
using namespace std;

#define RIPPLE_STRENGTH  8

class RippleBuffer;

/**
 * 水波纹效果图片
 */
class RippleSprite : public Sprite{
    
public:
    RippleSprite();
    virtual ~RippleSprite();
    
public:
    static RippleSprite* createWithGSLength(const string &file, float gsLength);
    virtual bool initWithGSLength(const string &file, float gsLength);
    
protected:
    void initGridSide();
    void initEvent();
    
    virtual void update(float dt) override;
    void updateOnce();
    
    void doTouch(const Vec2 &touchPoint,float depth, float r);
    void onDraw(const Mat4 &transform, uint32_t flags);
    
protected:
    virtual void draw(cocos2d::Renderer *renderer,const cocos2d::Mat4& transform,uint32_t flags) override;
    virtual void onEnter() override;
    virtual void onExit() override;
    
private:
    int _iRow;
    int _iCol;
    float _fTime;
    CC_SYNTHESIZE(float, _fGSLength, GSLength);
    float _fRippleStrength;
    
    Cmesh           *_pMesh;
    vector<Cv2>     _vTexCoordListStore;
    CindexVBO       *_pIndexVBO;
    RippleBuffer    *_pSrcBuffer;
    RippleBuffer    *_pDstBuffer;
    
    CustomCommand _customCommand;
    
};

class RippleBuffer : public Ref
{
public:
    vector<vector<float> > mat;
};

#endif /* RippleSprite_hpp */
