//
//  SmearColor.hpp
//  GLDemo
//
//  Created by tangbowen on 16/8/11.
//
//

#ifndef SmearColor_hpp
#define SmearColor_hpp

#include "SmearNode.hpp"

extern CC_DLL const GLchar *smearColorPositonTextureColor_frag;
#define kSmearColorShaderName "SmearColorShaderName"

class SmearColor : public SmearNode {
    
public:
    SmearColor();
    virtual ~SmearColor();
    
public:
    virtual void initShader() override;
    
public:
    virtual void onEnter() override;
    virtual void onExit() override;
    
};

#endif /* SmearColor_hpp */
