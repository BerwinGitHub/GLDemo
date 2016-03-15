//
//  GLHSVScene.hpp
//  GLDemo
//
//  Created by tangbowen on 11/3/16.
//
//

#ifndef GLHSLScene_hpp
#define GLHSLScene_hpp

#include "cocos2d.h"
#include "HSLSprite.h"
#include "extensions/cocos-ext.h"
#include "TestBaseScene.hpp"

using namespace cocos2d::extension;
USING_NS_CC;

class GLHSLScene : public TestBaseScene
{
public:
    GLHSLScene();
    static Scene *scene();
    virtual bool init() override;
    virtual string title() override { return "HSLSprite/HSVSprite"; } ;
    
    CREATE_FUNC(GLHSLScene);
    
protected:
    HSLSprite   *_pHSLSprite;
    void sliderChanged(Ref *ref, Control::EventType type);
    void sSliderChanged(Ref *ref, Control::EventType type);
    void lSliderChanged(Ref *ref, Control::EventType type);
    
};

#endif /* GLHSLScene_hpp */
