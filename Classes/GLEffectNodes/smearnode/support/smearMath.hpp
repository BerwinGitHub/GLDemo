//
//  smearMath.hpp
//  GameFrame
//
//  Created by 唐博文 on 16/8/6.
//
//

#ifndef smearMath_hpp
#define smearMath_hpp

#include "cocos2d.h"

using namespace cocos2d;
using namespace std;

class smearMath {
    
public:
    /**
     * @brief Interpolation between two point
     */
    inline static vector<Vec2> interpolate(Vec2 p1, Vec2 p2, float delta){
        float lDistance = p1.getDistance(p2);
        int lDis = (int)lDistance;
        Vec2 dif = p2 - p1;
        
        vector<Vec2> vPosition;
        for (int i = 0; i < lDis; i += delta) {
            float lDelta = float(i) / lDistance;
            
            Vec2 p(p1.x + (dif.x * lDelta), p1.y + (dif.y * lDelta));
            vPosition.push_back(p);
        }
        return vPosition;
    }
    
    /**
     * @brief 
     *  make a ray with a point<@param o> & <@param degress>
     *  get<@param count> position,
     *  gap each point space distance.
     *
     */
    inline static vector<Vec2> sprayRays(const Vec2 &o, float degrees, int count, float gap = 7.0f){
        float radians = CC_DEGREES_TO_RADIANS(degrees);
        float cosR = cos(radians);
        float sinR = sin(radians);
        
        vector<Vec2> vPoints;
        for (int i = 0; i < count; i++) {
            int r = i * gap;
            Vec2 raysPos = Vec2(r * cosR, r * sinR) + o;
            vPoints.push_back(raysPos);
        }
        return vPoints;
    }
    
};

#endif /* smearMath_hpp */
