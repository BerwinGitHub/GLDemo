/*
* Effects implemented by and only for fragment shader. 
*
* xfw,2014-3.<xfw5@163.com>
*/

#ifndef _SHADER_EFFECTS_H_
#define _SHADER_EFFECTS_H_

#include "cocos2d.h"

USING_NS_CC;

class ShaderNode : public Node
{
public:
    ShaderNode();
    ~ShaderNode();

	void setResolution(const float& w, const float& h);
    virtual void draw(Renderer *renderer, const kmMat4 &transform, bool transformUpdated) override;

protected:
	bool initWithVertex(const std::string &vert, const std::string &frag);
    void loadShaderVertex(const std::string &vert, const std::string &frag);

	virtual void buildCustomUniforms() = 0;
	virtual void setCustomUniforms() = 0;
    void onDraw(const kmMat4 &transform, bool transformUpdated);

    Vertex2F _resolution;

    GLuint	_uniformResolution;

    std::string _vertFileName;
    std::string _fragFileName;
    CustomCommand _customCommand;
};

typedef struct _MaskParams {
	_MaskParams(): inverted(0.0f), radius(0.0f), gradient(0.0f), \
					brightness(0.0f), color(Color4F::WHITE) {}
	_MaskParams(float i, float r, float g, float b, Color4F c) : inverted(i), \
					radius(r), gradient(g), brightness(b), color(c) {}
	
	/* 
	* Inverted mask, 0.0~1.0 
	* 0.0: un-inverted.
	* 1.0: inverted.
	* 0.5: 50% inverted, it make the inverted masker looks transparency.
	* In an word, when @inverted not eq 0.0, it just like the opacity.
	*/
	GLfloat inverted;

	/* masker radius, 0.0~1.0 */
	GLfloat radius;

	/* how fast falloff, greater than 0.5 is suggestion */
	GLfloat gradient;

	/* brightness of masker, greater than 3.0 is suggestion */
	GLfloat brightness;

	/*
	* masker color, if you want to keep the orig texture color.
	* Just set as white.
	*/
	Color4F color; 
}MaskParams;

/*
* Usage:
* Texture2D::TexParams tParams = { GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT};
* MaskParams mParams = MaskParams(0.0f, 0.5f, 1.0f, 3.0f, Color4F::WHITE);
* auto maskEffect = MaskShaderEffect::create("common.vs", "mask.fs", maskBackground, tParams, true);
* maskEffect->setMaskParams(mParams);
* maskEffect->setPosition(VisibleRect::center()); // screen centre.
* this->addChild(maskEffect);
*/
class MaskShaderEffect:public ShaderNode
{
public:
	/*
	* @vs: cocos2dx common vertex shader.
	* @fs: fragment shader.
	* @maskBackground: background image being masked.
	* @params: Texture params, how to mapping your texture.
	* @touchCtrl: touch event support. Get the touch point as the masker centre.
	*
	* Note:
	* The default contents size of @MaskShaderEffect is set as texture size.
	* You can call @setResolution to change the viewport/enlarge the texture size.
	*/
	static MaskShaderEffect * create(const std::string &vs, const std::string &fs, 
										const std::string &maskBackground, 
										Texture2D::TexParams &params, bool touchCtrl);
	/* Setup the masker centre. */
	void setMaskPosition(Point &p);

	/* setup masker coefficient */
	void setMaskParams(MaskParams &params);

	/* Touch event support. */
	void enableTouchCtrl(bool enable);
	
	MaskShaderEffect();
	~MaskShaderEffect();
private:
	bool initWithTexture(const std::string &texture, Texture2D::TexParams &params);
	virtual void buildCustomUniforms();
	virtual void setCustomUniforms();
	bool onTouchBegan(Touch* touches, Event* event);

#ifdef ES_30_SUPPORT
	/* Interface blocks supported in GLSL ES 3.00 only. */
	bool setupUBO(MaskParams &params);
#endif

	Vertex2F _touchPoint;
	MaskParams _maskParams;

	GLuint _uniformTouchPoint;

#ifdef ES_30_SUPPORT
	GLuint _uniformMaskParams;
#else
	GLuint _uniformCoefficient; /* hold: inverted, radius, gradient, brightness. */
	GLuint _uniformColor;
#endif
	
	GLuint _textureID;

	Texture2D *_texture;
};

#endif