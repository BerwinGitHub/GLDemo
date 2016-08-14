//
//  ScribbleArmatureNode.h
//  ScribbleDemo_3_2
//
//  Created by renhong on 14-10-29.
//
//

#ifndef __ScribbleDemo_3_2__ScribbleArmatureNode__
#define __ScribbleDemo_3_2__ScribbleArmatureNode__

#include <stdio.h>
#include "ScribbleTouchNode.h"
#include "cocostudio/CCBone.h"
using namespace cocostudio;
class ScribbleArmatureNode : public ScribbleTouchNode {
public:
    bool init(Size pCanvasSize);
    static ScribbleArmatureNode *create(Size pCanvasSize);
public:
    ScribbleArmatureNode();
    CC_DEPRECATED_ATTRIBUTE inline void setArmaturePosition(Point pPoint){_armaturePosition = pPoint;_haveArmaturePosition = true;}
    inline void setArmatureTransform(AffineTransform pTransformInWorld){_armatureTransformInWorld = pTransformInWorld;}
    inline Point positionInArmature(Point pPointInWorld){
        if (_haveArmaturePosition) {
            pPointInWorld.subtract(_armaturePosition);
            return pPointInWorld;
        }
        
        Vec2 lArmaturePos = Vec2(_armatureTransformInWorld.tx, _armatureTransformInWorld.ty);
        pPointInWorld.subtract(lArmaturePos);
        
        Vec2 lArmatureScale = Vec2(_armatureTransformInWorld.a, _armatureTransformInWorld.d);
        return Vec2(pPointInWorld.x / lArmatureScale.x, pPointInWorld.y / lArmatureScale.y);
    }
    void paint(Point pWorldPosition) override;
    void paint(Point pStartWorldPosition, Point pEndWorldPosition) override;
    void paint(vector<Point> pWorldPositions) override;
    
public://method for armature animation
    void setAdditionalTransform(Mat4* additionalTransform) override;
    void visit(Renderer* renderer, const Mat4 &parentTransform, uint32_t parentFlags) override;
    void updateArmatureTransform();
    void updateTransform() override;
    virtual void setSkinData(const BaseData &data);
    virtual const BaseData &getSkinData() const;
    virtual void setBone(Bone *bone);
    virtual Bone *getBone() const;
    
protected:
    Point _armaturePosition;
    AffineTransform _armatureTransformInWorld;
    BaseData _skinData;
    Bone *_bone;
//    Armature *_armature;
    cocos2d::Mat4 _skinTransform;
    CC_DEPRECATED_ATTRIBUTE bool _haveArmaturePosition;
};
#endif /* defined(__ScribbleDemo_3_2__ScribbleArmatureNode__) */
