//
//  Util.cpp
//  ScribbleDemo_3_2
//
//  Created by renhong on 4/2/15.
//
//

#include "ScribbleUtil.h"

//namespace Scribble {
ScribbleUtil *ScribbleUtil::s_SharedUtil = nullptr;
ScribbleUtil* ScribbleUtil::getInstance()
{
    if (!s_SharedUtil)
    {
        s_SharedUtil = new (std::nothrow) ScribbleUtil();
        CCASSERT(s_SharedUtil, "FATAL: Not enough memory");
    }
    
    return s_SharedUtil;
}

    Color4B ScribbleUtil::getColor(Image *pImage, Vec2 pPosition){
        unsigned char *data = pImage->getData();
        const int bytesPerPixel = pImage->getBitPerPixel() / 8;
        
        int row = pImage->getHeight() - pPosition.y;
        int col = pPosition.x;
        if (row < 0) {
            row = 0;
        }
        else if(row >= pImage->getHeight()){
            row = pImage->getHeight() - 1;
        }
        if (col < 0) {
            col = 0;
        }
        else if(col >= pImage->getWidth()){
            col = pImage->getWidth() - 1;
        }
        int index = row * pImage->getWidth() + col;
        index *= bytesPerPixel;
        unsigned char r = data[index];
        unsigned char g = data[index + 1];
        unsigned char b = data[index + 2];
        unsigned char a = (3 == bytesPerPixel) ? 255 : data[index + 3];
//        Debug.log("color: %d %d %d %d", r, g, b, a);
        
        return Color4B(r, g, b, a);
    }

bool ScribbleUtil::supportGLExtension(const char* name) {
    log("hasExtension start");
    const char* extensions = (const char*)glGetString(GL_EXTENSIONS);
    const char* end = extensions + strlen(extensions);
    size_t name_length = strlen(name);
    log("extensions %s", extensions);
    while( extensions < end ) {
        if (strchr(extensions, ' ') <= 0) {
            break;
        }
        size_t position = strchr(extensions, ' ') - extensions;
        log("position %ld", position);
        if( position == name_length &&
           strncmp(extensions, name, position) == 0 )
            return true;
        extensions += (position + 1);
    }
    return false;
}

long ScribbleUtil::getCurrentTimeByMicroseconds(){
    struct timeval tv;
    gettimeofday(&tv,NULL);
    return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}

Image *ScribbleUtil::getImage(Sprite *pSprite){
    RenderTexture *lRT = RenderTexture::create(pSprite->getContentSize().width, pSprite->getContentSize().height);
    pSprite->setPosition(pSprite->getContentSize() / 2);
    lRT->begin();
    pSprite->visit();
    lRT->end();
    return lRT->newImage();
}

vector<Point> ScribbleUtil::interpolate(Point pStartPosition, Point pEndPosition, float pDelta){
    float lDistance = pStartPosition.getDistance(pEndPosition);
    int lDis = (int)lDistance;
    
    vector<Point> lPositions;
    for (int i = 0; i < lDis; i += pDelta) {
        float lDelta = float(i) / lDistance;
        float lDifX = pEndPosition.x - pStartPosition.x;
        float lDifY = pEndPosition.y - pStartPosition.y;
        
        Point lPosition(pStartPosition.x + (lDifX * lDelta), pStartPosition.y + (lDifY * lDelta));
        lPositions.push_back(lPosition);
    }
    return lPositions;
}

GLProgram *ScribbleUtil::loadGLProgram(const GLchar * pVertProgram, const GLchar *pFragProgram, string pGLProgramKey){
    GLProgram *p = GLProgram::createWithByteArrays(pVertProgram, pFragProgram);
    CHECK_GL_ERROR_DEBUG();
    if (nullptr != p) {
        GLProgramCache::getInstance()->addGLProgram(p, pGLProgramKey);
    }
    
    return p;
}

void ScribbleUtil::setGLProgram(Node *n, GLProgram *p) {
    n->setGLProgram(p);
    if (n->getChildren().size() <= 0) return;
    Vector<Node *> children = n->getChildren();
    for (Vector<Node *>::iterator itor = children.begin(); itor != children.end(); ++itor) {
        Node *lNode = *itor;
        if (lNode) {
            setGLProgram(lNode, p);
        }
    }
}
//}
