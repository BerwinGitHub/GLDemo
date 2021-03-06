//
//  SmearNode.hpp
//  GLDemo
//
//  Created by tangbowen on 16/7/28.
//
//

#ifndef SmearNode_hpp
#define SmearNode_hpp

#include "cocos2d.h"

USING_NS_CC;
using namespace std;

extern CC_DLL const GLchar *smearPositonTextureColor_frag;
#define kSmearShaderName "SmearShaderName"

typedef Sprite Target;  // smear need shown obj
typedef Sprite Paint;   // smear paint

class SmearNode : public RenderTexture {
    
public:
    SmearNode();
    virtual ~SmearNode();
    
public:
    enum class PaintType {
        kPaint,
        kEaser
    };
    
public:
    static SmearNode* create(const Target *t, const Paint *p);
    static SmearNode* create(const string &tFile, const string &pFile);
    virtual bool init(const Target *t, const Paint *p);
    
    virtual void draw(const Vec2 &pos);
    virtual void draw(const Vec2 &startPos, const Vec2 &endPos);
    virtual void draw(vector<Vec2> pWorldPositions);
    
    virtual void drawSelf();
    virtual void clearSelf();
    
public:
    /**
     * @brief paint type getter & setter.
     */
    virtual void setPaintType(SmearNode::PaintType t);
    PaintType getPaintType();
    
    /**
     * @brief paint soft transition value getter & setter.
     */
    virtual void setPaintHardness(float hard);
    float getPaintHardness();
    
    /**
     * @brief paint texture getter & setter.
     */
    virtual void setPaint(Texture2D* tex);
    virtual void setPaint(const string &file);
    Paint *getPaint();
    
    /**
     * @brief target texture getter & setter.
     */
    virtual void setTarget(Texture2D* tex);
    virtual void setTarget(const string &file);
    Target *getTarget();
    
    /**
     * @brief antiAliasing & disAntiAliasing.
     */
    virtual void antiAliasing();
    virtual void disAntiAliasing();
    virtual void setAntiAliasingPaint(bool enable);
    
protected:
    virtual void initShader();
    
    void bindPaintPosition(Vec2 p);
    void bindPaintTexture();
    void bindTargetTexture();
    
    void bindPaintAlphaValue(float v);
    void bindTargetAlphaValue(float v);
    void bindAntiAliasingPaint(bool b);
    
    void setGLProgram(Node *n, GLProgram *p);
    
protected:
    Target      *_pTarget;
    Paint       *_pPaint;
    PaintType   _ePaintType;
    
    bool        _bAntiAliasingPaint;
    float       _fPaintHardness;
    
    CC_SYNTHESIZE(Size, _sTargetMiddle, TargetMiddle);
    CC_SYNTHESIZE(Size, _sPaintMiddle, PaintMiddle);
    CC_SYNTHESIZE(Size, _sCanvaSize, CanvaSize);
    
public:
    virtual void onEnter() override;
    virtual void onExit() override;
};


#endif /* SmearNode_hpp */
