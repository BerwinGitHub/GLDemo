//
//  ColoringNode.cpp
//  DrawDemo
//
//  Created by renhong on 9/15/14.
//
//

#include "ColoringNode.h"

#include "ScribbleUtil.h"
#define kShaderName "Coloring.frag"
#define STRINGIFY(A)  #A
#include kShaderName

ColoringNode *ColoringNode::create(string pTemplateFile, string pLineArtFile){
    ColoringNode *ret = new ColoringNode();
    if(ret && ret->init(pTemplateFile, pLineArtFile)){
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

ColoringNode::ColoringNode() : _coloringMode(ColoringNode::ColoringMode::eFlat){
    _template = nullptr;
    _lineArt = nullptr;
    _canvas = nullptr;
    _colorTexture = nullptr;
    _endColorTexture = nullptr;
    _colorModelTexture = nullptr;
    _templateImage = nullptr;
    _canvasDefaultColor = Color4F::WHITE;
    _shapeBoundTexture = nullptr;
    _prePatternColor = Color4B::BLACK;
    ignoreOutOfLineframe = false;
}

ColoringNode::~ColoringNode(){
    this->setTemplate(nullptr);
//    CC_SAFE_RELEASE(_lineArt);
//    CC_SAFE_RELEASE(_canvas);
    CC_SAFE_RELEASE(_colorTexture);
    CC_SAFE_RELEASE(_endColorTexture);
    CC_SAFE_RELEASE(_colorModelTexture);
    CC_SAFE_RELEASE(_templateImage);
    CC_SAFE_RELEASE(_shapeBoundTexture);
}

bool ColoringNode::init(string pTemplateFile, string pLineArtFile){
    if (!Node::init()) {
        return false;
    }
    
    //构建并初始化画布
    Sprite *lTemplate = Sprite::create(pTemplateFile.c_str());
    this->setTemplate(lTemplate);
    this->setContentSize(lTemplate->getContentSize());
    Size drawSize = lTemplate->getContentSize();
    lTemplate->setPosition(Vec2(drawSize.width/2, drawSize.height/2));
    //调试Pattern的时候可以打开下行代码
//    this->addChild(_template, (int)ColoringNode::ColoringZOrder::eZPattern);
    _lineArt = Sprite::create(pLineArtFile);
    this->addChild(_lineArt, (int)ColoringNode::ColoringZOrder::eZLineFrame);
    _lineArt->setPosition(Vec2(drawSize.width / 2, drawSize.height / 2));
//    _lineArt->setVisible(false);
    
    
    
    this->initCanvas(drawSize);
    this->initRenderPara();
    this->uploadRenderPara();
    this->initPattern(pTemplateFile);
    
    return true;
}

void ColoringNode::initCanvas(Size pCanvasSize){
    _canvas = RenderTexture::create(pCanvasSize.width, pCanvasSize.height);
//    _canvas->retain();
    _canvas->setPosition(pCanvasSize.width / 2, pCanvasSize.height / 2);
    this->addChild(_canvas, (int)ColoringNode::ColoringZOrder::eZCanvas);
    
//    _canvas->clear(1.0, 1.0, 1.0, 1.0);
    _canvas->getSprite()->getTexture()->setAntiAliasTexParameters();
    _canvas->begin();
//    _lineArt->visit();
    _canvas->end();
}

void ColoringNode::initRenderPara(){
    //构造着色参数
    _colorTexture = new Texture2D;
    _endColorTexture = new Texture2D;
    _colorModelTexture = new Texture2D;
}

void ColoringNode::uploadRenderPara(){
    const int texSize = gTextureSize;
    const int texLenth = texSize * texSize * 4;
    unsigned char texData[texLenth];
    memset(texData, 0x0, sizeof(unsigned char) * texLenth);
    _colorTexture->setAliasTexParameters();
    _colorTexture->initWithData(texData, texLenth, Texture2D::PixelFormat::RGBA8888, texSize, texSize, Size(texSize, texSize));
    
    memset(texData, 0x0, sizeof(unsigned char) * texLenth);
    _endColorTexture->setAliasTexParameters();
    _endColorTexture->initWithData(texData, texLenth, Texture2D::PixelFormat::RGBA8888, texSize, texSize, Size(texSize, texSize));
    
    memset(texData, 0xFF, sizeof(unsigned char) * texLenth);
    _colorModelTexture->setAliasTexParameters();
    _colorModelTexture->initWithData(texData, texLenth, Texture2D::PixelFormat::RGBA8888, texSize, texSize, Size(texSize, texSize));
    
    setBrushShader();
    
    GLProgramState *lState = GLProgramState::getOrCreateWithGLProgram(_template->getGLProgram());
    lState->setUniformTexture("s_texture_color", _colorTexture);
    lState->setUniformTexture("s_texture_color_end", _endColorTexture);
    lState->setUniformTexture("s_texture_color_model", _colorModelTexture);
}

void ColoringNode::initPattern(string pPatternImage){
    GLProgramState *lState = GLProgramState::getOrCreateWithGLProgram(_template->getGLProgram());
    lState->setUniformTexture("s_texture_shape", _template->getTexture());
    lState->setUniformVec2("v_texture_shape_size", _template->getTexture()->getContentSize());
    CC_SAFE_RELEASE(_templateImage);
    _templateImage = new Image();
    _templateImage->initWithImageFile(pPatternImage.c_str());
    _shapeBoundTexture = this->getShapeBounds(_templateImage);
    _shapeBoundTexture->retain();
    lState->setUniformTexture("s_texture_shape_bounds", _shapeBoundTexture);
}

void ColoringNode::setBrushShader(){
    string lGLProgramKey = ScribbleUtil::getInstance()->createUniqueGLProgramName(kShaderName, this);
    GLProgram *lProgram = ScribbleUtil::getInstance()->loadGLProgram(ccPositionTextureColor_noMVP_vert, Coloring_frag, lGLProgramKey);
    if (NULL != lProgram) {
        if (_template->getGLProgram() != lProgram) {
            ScribbleUtil::getInstance()->setGLProgram(_template, lProgram);
        }
    }
}

//检测是否点中线框图内部
bool ColoringNode::touchedInsideLineframe(Vec2 pWorldPosition){
    if (nullptr == _templateImage) {
        return false;
    }
    if (!this->getIgnoreOutOfLineframe()) {//如果不忽略线框图外部区域，则线框图外部区域可以填色，认为本点击处于线框图内部。
        return true;
    }
    Point pos = this->convertToNodeSpace(pWorldPosition);
    
    Color4B lTouchedColor = ScribbleUtil::getInstance()->getColor(_templateImage, pos);
    
    Color4B lLeftTopColor = ScribbleUtil::getInstance()->getColor(_templateImage, Vec2(0, _templateImage->getHeight()));
    if (lTouchedColor == lLeftTopColor) {
        return false;
    }
    Color4B lLeftBottomColor = ScribbleUtil::getInstance()->getColor(_templateImage, Vec2(0, 0));
    if (lTouchedColor == lLeftBottomColor) {
        return false;
    }
    
    Color4B lRightTopColor = ScribbleUtil::getInstance()->getColor(_templateImage, Vec2(_templateImage->getWidth(), _templateImage->getHeight()));
    if (lTouchedColor == lRightTopColor) {
        return false;
    }
    
    Color4B lRightBottomColor = ScribbleUtil::getInstance()->getColor(_templateImage, Vec2(_templateImage->getHeight(), 0));
    if (lTouchedColor == lRightBottomColor) {
        return false;
    }
    
    return true;
}

bool ColoringNode::coloring(Point pWorldPosition){
    if (!this->touchedInsideLineframe(pWorldPosition)) {
        return false;
    }
    
    if (nullptr == _templateImage) {
        return false;
    }
    Point lPosInColoringNode = this->convertToNodeSpace(pWorldPosition);
    Rect lColoringNodeRect = Rect(0, 0, _contentSize.width, _contentSize.height);
    if (!lColoringNodeRect.containsPoint(lPosInColoringNode)) {
        return false;
    }
    Color4B lTouchedColor = ScribbleUtil::getInstance()->getColor(_templateImage, lPosInColoringNode);
    if (_prePatternColor.r == lTouchedColor.r && _prePatternColor.g == lTouchedColor.g) {
        return false;
    }
    _prePatternColor.r = lTouchedColor.r;
    _prePatternColor.g = lTouchedColor.g;
//    log("touched %d %d, color %d %d %d", (int)pos.x, (int)pos.y, lTouchedColor.r, lTouchedColor.g, lTouchedColor.b);
    
    const int texSize = gPixelSize;
    const int texLenth = texSize * texSize * 4;
    
    int lX = (lTouchedColor.r - texSize / 2);
    lX = lX < 0 ? 0 : lX;
    int lY = (lTouchedColor.g - texSize / 2);
    lY = lY < 0 ? 0 : lY;
    
    unsigned char tmp[texLenth];
    for (int i = 0; i < texSize * texSize; ++i) {
        tmp[i * 4 + 0] = _startColor.r;
        tmp[i * 4 + 1] = _startColor.g;
        tmp[i * 4 + 2] = _startColor.b;
        tmp[i * 4 + 3] = _startColor.a;
    }
    _colorTexture->updateWithData(tmp, lX, lY, texSize, texSize);
    
    if (ColoringMode::eFlat != _coloringMode) {
        for (int i = 0; i < texSize * texSize; ++i) {
            tmp[i * 4 + 0] = _endColor.r;
            tmp[i * 4 + 1] = _endColor.g;
            tmp[i * 4 + 2] = _endColor.b;
            tmp[i * 4 + 3] = _endColor.a;
        }
    }
    _endColorTexture->updateWithData(tmp, lX, lY, texSize, texSize);
    
    int hSmooth = 0;
    int vSmooth = 0;
    switch (_coloringMode) {
        case ColoringMode::eHorizonSmooth:{
            hSmooth = 1;
            break;
        }
        case ColoringMode::eVerticalSmooth:{
            vSmooth = 1;
            break;
        }
        case ColoringMode::eDiagonalSmooth:{
            hSmooth = 1;
            vSmooth = 1;
            break;
        }
        default:
            break;
    }
    memset(tmp, 0, sizeof(texLenth));
    for (int i = 0; i < texSize * texSize; ++i) {
        tmp[i * 4 + 0] = hSmooth * 255;
        tmp[i * 4 + 1] = vSmooth * 255;
    }
    _colorModelTexture->updateWithData(tmp, lX, lY, texSize, texSize);
    
    //        _canvas->clear(1, 1, 1, 1);
    _canvas->begin();
    Texture2D::TexParams tp;
    tp.magFilter = GL_NEAREST;
    tp.minFilter = GL_NEAREST;
    tp.wrapS = GL_CLAMP_TO_EDGE;
    tp.wrapT = GL_CLAMP_TO_EDGE;
    _template->getTexture()->setTexParameters(tp);
    _template->visit();
    //        if (nullptr != _lineArt) {
    //            _lineArt->visit();
    //        }
    _canvas->end();
    Director::getInstance()->getRenderer()->render();
    return true;
}

Texture2D *ColoringNode::getShapeBounds(Image *pShapeImage){//获取待着色图片内容的边界，记录边界的左下角和右上角坐标。
    unsigned char *imageData = pShapeImage->getData();
    const int height = pShapeImage->getHeight();
    const int width = pShapeImage->getWidth();
    const int bytePerPixel = pShapeImage->getBitPerPixel() / 8;
    map<unsigned int, ColoringArea> colorAreaInfo;
    for (int row = 0; row < height; ++row) {
        for (int col = 0; col < width; ++col) {
            int index = row * width + col;
            index *= bytePerPixel;
            unsigned int color;
            memset(&color, 255, sizeof(unsigned int));//因为有可能是RGB格式，需要要对Alpha通道设置默认值，使用255使图片不透明。
            memcpy(&color, imageData + index, bytePerPixel);
            map<unsigned int, ColoringArea>::iterator itor = colorAreaInfo.find(color);
            if (colorAreaInfo.end() == itor) {
                ColoringArea colorArea(col, col, row, row);
                pair<map<unsigned int, ColoringArea>::iterator, bool> res = colorAreaInfo.insert(pair<unsigned int, ColoringArea>(color, colorArea));
                if (res.second) {
                    itor = res.first;
                }
            }
            ColoringArea *colorArea = &(itor->second);
            if (col < colorArea->left) {
                colorArea->left = col;
            }
            if (col > colorArea->right) {
                colorArea->right = col;
            }
            if (row < colorArea->top) {
                colorArea->top = row;
            }
            if (row > colorArea->bottom) {
                colorArea->bottom = row;
            }
        }
    }
    
    const int texSize = gTextureSize;
    const int texLenth = texSize * texSize * 4;
    unsigned char texData[texLenth];
    memset(texData, 0x0, sizeof(unsigned char) * texLenth);
    Texture2D *colorTexture = new Texture2D;
    colorTexture->setAliasTexParameters();
    colorTexture->initWithData(texData, texLenth, Texture2D::PixelFormat::RGBA8888, texSize, texSize, Size(texSize, texSize));
    for (map<unsigned int, ColoringArea>::iterator itor = colorAreaInfo.begin(); itor != colorAreaInfo.end(); ++itor) {
        ColoringArea colorArea = itor->second;
        unsigned int tmp = itor->first;
//        unsigned char a = tmp >> 24;
//        unsigned char b = (tmp >> 16) & 0x00FF;
        unsigned char g = (tmp >> 8) & 0x00FF;
        unsigned char r = (tmp & 0x00FF);
        
        const int index = g * texSize + r;
        texData[index] = colorArea.left * 255.0 / pShapeImage->getWidth();
        texData[index + 1] = colorArea.right * 255.0 / pShapeImage->getWidth();
        texData[index + 2] = colorArea.top * 255.0 / pShapeImage->getHeight();
        texData[index + 3] = colorArea.bottom * 255.0 / pShapeImage->getHeight();
        
        const int texSize = gPixelSize;
        const int texLenth = texSize * texSize * 4;
        unsigned char tmp1[texLenth];
        for (int i = 0; i < texSize * texSize; ++i) {
            tmp1[i * 4 + 0] = texData[index];
            tmp1[i * 4 + 1] = texData[index + 1];
            tmp1[i * 4 + 2] = texData[index + 2];
            tmp1[i * 4 + 3] = texData[index + 3];
        }
        
        int lX = (r - texSize / 2);
        lX = lX < 0 ? 0 : lX;
        int lY = (g - texSize / 2);
        lY = lY < 0 ? 0 : lY;
        
        colorTexture->updateWithData(tmp1, lX, lY, texSize, texSize);
    }
    
//    Texture2D::TexParams texParams = {(GLuint)(GL_LINEAR),GL_LINEAR,GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE};
//    colorTexture->setTexParameters(texParams);
    colorTexture->autorelease();
    return colorTexture;
}

void ColoringNode::reset(){
    this->_canvas->clear(_canvasDefaultColor.r, _canvasDefaultColor.g, _canvasDefaultColor.b, _canvasDefaultColor.a);
    //重置着色参数
    const int texSize = gTextureSize;
    const int texLenth = texSize * texSize * 4;
    unsigned char texData[texLenth];
    memset(texData, 0xFF, sizeof(unsigned char) * texLenth);
    _colorTexture->setAliasTexParameters();
    _colorTexture->updateWithData(texData, 0, 0, texSize, texSize);
    
    memset(texData, 0xFF, sizeof(unsigned char) * texLenth);
    _endColorTexture->setAliasTexParameters();
    _endColorTexture->updateWithData(texData, 0, 0, texSize, texSize);
    
    memset(texData, 0xFF, sizeof(unsigned char) * texLenth);
    _colorModelTexture->setAliasTexParameters();
    _endColorTexture->updateWithData(texData, 0, 0, texSize, texSize);
    
    GLProgramState *lState = GLProgramState::getOrCreateWithGLProgram(_template->getGLProgram());
    lState->setUniformTexture("s_texture_color", _colorTexture);
    lState->setUniformTexture("s_texture_color_end", _endColorTexture);
    lState->setUniformTexture("s_texture_color_model", _colorModelTexture);
}