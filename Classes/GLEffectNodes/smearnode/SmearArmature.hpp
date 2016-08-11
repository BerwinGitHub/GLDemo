//
//  SmearArmature.hpp
//  GameFrame
//
//  Created by 唐博文 on 16/8/7.
//
//

#ifndef SmearArmature_hpp
#define SmearArmature_hpp

#include "SmearNode.hpp"
#include "cocostudio/CCBone.h"

using namespace cocostudio;

class SmearArmature : public SmearNode {
    
public:
    SmearArmature();
    virtual ~SmearArmature();
    
public:
    static SmearArmature* createWithBone(Bone* b, const Target *t, const Paint *p);
    static SmearArmature* createWithBone(Bone* b, const string &tFile, const string &pFile);
    virtual bool initWithBone(Bone* b, const Target *t, const Paint *p);
    
    virtual void draw(const Vec2 &pos) override;
    virtual void draw(const Vec2 &startPos, const Vec2 &endPos) override;
    virtual void draw(vector<Vec2> worldPositions) override;
    
    virtual void setBone(Bone *b);
    virtual Bone *getBone() const;
    
public:
    virtual void visit(cocos2d::Renderer *render, const cocos2d::Mat4 &transform, uint32_t flag) override;
    virtual void updateTransform() override;
    virtual void setAdditionalTransform(Mat4* additionalTransform) override;
    
public:
    virtual void onEnter() override;
    virtual void onExit() override;
    
private:
    virtual void setSkinData(const BaseData &data);
    virtual const BaseData &getSkinData() const;
    
    void updateArmatureTransform();
    Vec2 positionInArmature(const Vec2 &worldPos);
    
    Target *getTargetByBone(Bone *b);
    
private:
    Bone            *_pBone;
    
    BaseData        _skinData;
    cocos2d::Mat4   _skinTransform;
    
public:
    AffineTransform _armatureTransformInWorld;
    
};

#endif /* SmearArmature_hpp */
