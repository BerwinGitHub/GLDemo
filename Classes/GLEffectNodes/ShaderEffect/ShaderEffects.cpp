/*
* Effects implemented by and only for fragment shader. 
*
* xfw,2014-3.<xfw5@163.com>
*/

#include "ShaderEffects.h"

ShaderNode::ShaderNode()
:_resolution(Vertex2F(0.0f, 0.0f))
,_uniformResolution(0)
{
}

ShaderNode::~ShaderNode()
{
}

bool ShaderNode::initWithVertex(const std::string &vs, const std::string &fs)
{
    loadShaderVertex(vs, fs);

    _vertFileName = vs;
    _fragFileName = fs;

    return true;
}

void ShaderNode::setResolution(const float& w, const float& h)
{
	_resolution = Vertex2F(w, h);
	setContentSize(Size(w, h));

	setAnchorPoint(Point(0.5f, 0.5f));
}

void ShaderNode::loadShaderVertex(const std::string &vs, const std::string &fs)
{
    auto shader = new GLProgram();
    shader->initWithFilenames(vs, fs);
	this->setShaderProgram(shader);
	shader->release();

	CHECK_GL_ERROR_DEBUG();

    shader->bindAttribLocation("a_position", GLProgram::VERTEX_ATTRIB_POSITION);
	CHECK_GL_ERROR_DEBUG();

    shader->link();
	CHECK_GL_ERROR_DEBUG();

    shader->updateUniforms();
	CHECK_GL_ERROR_DEBUG();

    _uniformResolution = shader->getUniformLocation("resolution");

	this->buildCustomUniforms();
	CHECK_GL_ERROR_DEBUG();
}

void ShaderNode::draw(Renderer *renderer, const kmMat4 &transform, bool transformUpdated)
{
    _customCommand.init(_globalZOrder);
    _customCommand.func = CC_CALLBACK_0(ShaderNode::onDraw, this, transform, transformUpdated);
    renderer->addCommand(&_customCommand);
}

void ShaderNode::onDraw(const kmMat4 &transform, bool transformUpdated)
 {
    auto shader = getShaderProgram();
    shader->use();
    shader->setUniformsForBuiltins(transform);
    shader->setUniformLocationWith2f(_uniformResolution, _resolution.x, _resolution.y);
    
     //glUniform1f(_uniformTime, _time); // We uses the build-in @CC_Time[4]

	this->setCustomUniforms();
    
    GL::enableVertexAttribs( cocos2d::GL::VERTEX_ATTRIB_FLAG_POSITION );

	/* Draw viewport with two triangles */
	GLfloat vertices[12] = {0, 0, _resolution.x, 0, _resolution.x, _resolution.y,
							0, 0, 0, _resolution.y, _resolution.x,_resolution.y};

    glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 2, GL_FLOAT, GL_FALSE, 0, vertices);
    
    glDrawArrays(GL_TRIANGLES, 0, 6);
    
    CC_INCREMENT_GL_DRAWN_BATCHES_AND_VERTICES(1,6);
}

MaskShaderEffect::MaskShaderEffect()
	:_touchPoint(Vertex2F(0.0f, 0.0f))
	,_uniformTouchPoint(0)
	,_uniformCoefficient(0)
	,_maskParams()
	,_textureID(0)
	,_texture(NULL)
{
}


MaskShaderEffect::~MaskShaderEffect()
{
}

MaskShaderEffect* MaskShaderEffect::create(const std::string &vs, const std::string &fs, 
										   const std::string &maskBackground, 
										   Texture2D::TexParams &params, bool touchCtrl)
{
	MaskShaderEffect *sn = new MaskShaderEffect();
	if (sn && sn->init()) {
		sn->initWithTexture(maskBackground, params);
		sn->initWithVertex(vs, fs);
		sn->enableTouchCtrl(touchCtrl);
		sn->autorelease();
		return sn;
	}else {
		delete sn;
		sn = NULL;
		return NULL;
	}
}

bool MaskShaderEffect::initWithTexture(const std::string &texture, Texture2D::TexParams &params)
{
	_texture = Director::getInstance()->getTextureCache()->getTextureForKey(texture);
	if (_texture) {
		_textureID = _texture->getName();

		if (params.minFilter == GL_LINEAR_MIPMAP_LINEAR)
			_texture->generateMipmap();

		_texture->setTexParameters(params);

		float w = _texture->getContentSize().width;
		float h = _texture->getContentSize().height;

		/*
		* Set the shader viewport as the texture contents size.
		* You can set as the screen size, like:
		* setResolution(screen.width, screen.height);
		* At this solution, the texture will mapping as what you have
		* set before,like { GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT};
		* So, the texture params is important.
		*/
		this->setResolution(w, h);
		this->setMaskPosition(Point(w*0.5f,h*0.5f));

		return true;
	}

	log("Please make sure texture[%s] was pre-load to the cache.", texture);
	return false;
}

void MaskShaderEffect::setMaskPosition(Point &p)
{
	Point q = convertToNodeSpaceAR(p);
	if (abs(q.x) > _resolution.x*0.5 ||
		abs(q.y) > _resolution.y*0.5)
		log("warning: mask position out of contents size!");

	/* We setup the @_touchPoint as the mask position. */
	_touchPoint.x = p.x;
	_touchPoint.y = p.y;
}

/*
* UBO:
* uniform MaskParams{
*   float radius;
*   float gradient;
*   float brightness;
*   vec4 color;
* };
*/
#ifdef ES_30_SUPPORT
bool MaskShaderEffect::setupUBO(MaskParams & params)
{
	GLuint program = getShaderProgram()->getProgram();
	GLuint blockIdx = glGetUniformBlockIndex(program, "MaskParams");

	GLint blockSize;
	glGetActiveUniformBlockiv(program, blockIdx, GL_UNIFORM_BLOCK_DATA_SIZE, &blockSize);
	
	GLubyte *blockBuffer = (GLubyte*)malloc(blockSize);
	if (!blockBuffer) 
		return;
	else
		memset(blockBuffer, 0, sizeof(blockBuffer));

	const GLchar *name[] = {"radius", "gradient", "brightness", "color"};
	GLuint indices[4]; // indices[ARRAY_SIZE(name)];
	glGetUniformIndices(program, 4, name, indices);

	GLint offset[4];
	glGetActiveUniformsiv(program, 4, indices, GL_UNIFORM_OFFSET, offset);

	memcpy(blockBuffer+offset[0], &params.radius, sizeof(float));
	memcpy(blockBuffer+offset[1], &params.gradient, sizeof(float));
	memcpy(blockBuffer+offset[2], &params.brightness, sizeof(float));
	memcpy(blockBuffer+offset[3], &params.color, 4*sizeof(float));

	GLuint uboBuffer;
	glGenBuffers(1, &uboBuffer);
	glBindBuffer(GL_UNIFORM_BUFFER, uboBuffer);
	glBufferData(GL_UNIFORM_BUFFER, blockSize, blockBuffer, GL_STATIC_DRAW);

	glBindBufferBase(GL_UNIFORM_BUFFER, blockIdx, uboBuffer);

	free(blockBuffer);
	glDeleteBuffers(1, &uboBuffer);
}
#endif

void MaskShaderEffect::setMaskParams(MaskParams &params)
{
	memcpy(&_maskParams, &params, sizeof(params));

#ifdef ES_30_SUPPORT
	if (!setupUBO(params))
		log("warning:setup UBO failed!");
#endif
}

bool MaskShaderEffect::onTouchBegan(Touch* touch, Event* event)
{
	auto point = touch->getLocation();

	this->setMaskPosition(point);

	return true;
}

void MaskShaderEffect::enableTouchCtrl(bool enable)
{
	if (enable) {
		auto dispatcher = Director::getInstance()->getEventDispatcher();
		auto listener = EventListenerTouchOneByOne ::create();
		listener->onTouchBegan = CC_CALLBACK_2(MaskShaderEffect::onTouchBegan, this);
		dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	}
}

void MaskShaderEffect::buildCustomUniforms()
{
	GLProgram *shader = getShaderProgram();
	_uniformTouchPoint = shader->getUniformLocation("touchPoint");

#ifndef ES_30_SUPPORT
	_uniformCoefficient = shader->getUniformLocation("coefficient");
	_uniformColor = shader->getUniformLocation("color");
#endif
}

void MaskShaderEffect::setCustomUniforms()
{
	GLProgram *shader = getShaderProgram();
	shader->setUniformLocationWith2f(_uniformTouchPoint, _touchPoint.x, _touchPoint.y);

#ifndef ES_30_SUPPORT
	shader->setUniformLocationWith4f(_uniformCoefficient, _maskParams.inverted, \
					_maskParams.radius, _maskParams.gradient, _maskParams.brightness);
	shader->setUniformLocationWith4f(_uniformColor, _maskParams.color.r, \
					_maskParams.color.g, _maskParams.color.b, _maskParams.color.a);
#endif

	if (_textureID != 0) {
		/* cocos2dx alway binding GL_TEXTURE0 with 2d samlper/CC_Texture0. */
		glActiveTexture(GL_TEXTURE0);
		GL::bindTexture2D(_textureID);
	}
}