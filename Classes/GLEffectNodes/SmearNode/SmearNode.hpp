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

extern CC_DLL const GLchar *SmearShadeFrag;
#define kSmearShaderName "SmearShadeName"

typedef Sprite Target;
typedef Sprite Paint;
typedef RenderTexture Canvas;

class SmearNode : public Node {
    
public:
    SmearNode();
    virtual ~SmearNode();
    
public:
    enum class PaintType {
        kPaint,
        kEaser,
        kEaserFade
    };
    
public:
    static SmearNode* create(const Target *t, const Paint *p);
    virtual bool init(const Target *t, const Paint *p);
    
    void setPaintType(SmearNode::PaintType t);
    
    void draw(Vec2 pPosition);
    void draw(Vec2 pStartPosition, Vec2 pEndPosition);
    void draw(vector<Vec2> pWorldPositions);
    
    void setPaintTexture(Texture2D* tex);
    void setTargetTexture(Texture2D* tex);
    
    PaintType getPaintType();
    
private:
    void initShader();
    
    void bindPaintPosition(Vec2 p);
    void bindPaintAlphaValue(float v);
    void bindTargetAlphaValue(float v);
    void bindTargetSolid(bool s, float r = 1.0f);
    
    void bindAntiAliasingPaint(bool b);
    
    void bindPaintTexture();
    void bindTargetTexture();
    
private:
    Target  *_pTarget;
    Paint   *_pPaint;
    Canvas  *_pCanvas;
    PaintType _ePaintType;
    
    bool    _bAntiAliasingPaint;
    
public:
    virtual void onEnter() override;
    virtual void onExit() override;
    
private:
    string createUniqueGLProgramName(string name, void *p);
    GLProgram *loadGLProgram(const GLchar *vert, const GLchar *frag, string key);
    void setGLProgram(Node *n, GLProgram *p);
    
    vector<Vec2> interpolate(Vec2 pStartPosition, Vec2 pEndPosition, float pDelta);
    
};


#endif /* SmearNode_hpp */
