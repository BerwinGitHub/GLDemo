//
//  BlurHelper.h
//  Follow
//
//  Created by 唐博文 on 15/12/28.
//
//

#ifndef BlurHelper_h
#define BlurHelper_h

#include "cocos2d.h"

USING_NS_CC;

class BlurHelper {
    
public:
    /**
     * @brief 模糊屏幕指定Rect
     * @param rect 需要模糊的区域
     * @param blurRadius 模糊度(1-254)
     */
    static Sprite* blurNodeRect(Node* target, Rect rect, int blurRadius = 50);
    
    /**
     * @brief 模糊整个屏幕
     * @param blurRadius 模糊度(1-254)
     */
    static Sprite* blurScreen(int blurRadius = 50);
    
    /**
     * @brief 模糊屏幕指定Node的Rect区域
     * @param target 需要模糊的target Node区域
     * @param blurRadius 模糊度(1-254)
     */
    static Sprite* blurScreenNode(Node* target, int blurRadius = 50);
    
    
    static Sprite* blurScreenNodeWithClipping(Node* target, Vec2 origin, int blurRadius = 50);
    
private:
    static Sprite* renderNodeRect(Node* target, Rect rect = Rect::ZERO);
    
public:
    BlurHelper();
    ~BlurHelper();
    
};

#endif /* BlurHelper_h */
