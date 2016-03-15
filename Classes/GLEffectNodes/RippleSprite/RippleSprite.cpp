//
//  RippleSprite.cpp
//  Demo
//
//  Created by tangbowen on 10/3/16.
//
//

#include "RippleSprite.hpp"
#include "../support/GLProgramUnifos.h"

RippleSprite::RippleSprite():
_iRow(0),
_iCol(0),
_fTime(0.0f),
_pMesh(nullptr),
_fGSLength(0.0f),
_pIndexVBO(nullptr),
_pSrcBuffer(nullptr),
_pDstBuffer(nullptr)
{
    
}

RippleSprite::~RippleSprite()
{
    
}

RippleSprite* RippleSprite::createWithGSLength(const string &file, float gsLength)
{
    RippleSprite *pRet = new RippleSprite();
    if (pRet && pRet->initWithGSLength(file, gsLength)) {
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_RELEASE_NULL(pRet);
    return nullptr;
}

bool RippleSprite::initWithGSLength(const string &file, float gsLength)
{
    if (!Sprite::initWithFile(file)) {
        return false;
    }
    _fGSLength = gsLength;
    this->initGridSide();
    this->initEvent();
    this->scheduleUpdate();
    
    return true;
}

void RippleSprite::initEvent()
{
    EventListenerTouchOneByOne *e = EventListenerTouchOneByOne::create();
    e->setSwallowTouches(true);
    e->onTouchBegan = [=](Touch *t, Event *e){
        if (!this->isVisible()) {
            return false;
        }
        Rect worldBoundingBox = RectApplyAffineTransform(Rect(0, 0, this->getContentSize().width, this->getContentSize().height), this->getNodeToWorldAffineTransform());
        if (worldBoundingBox.containsPoint(t->getLocation())) {
            this->doTouch(t->getLocation(), 512, 12);
            return true;
        }
        return false;
    };
    e->onTouchMoved = [=](Touch *t, Event *e){
        this->doTouch(t->getLocation(), 512, 12);
    };
    e->onTouchEnded = [=](Touch *t, Event *e){};
    e->onTouchCancelled = [=](Touch *t, Event *e){};
    _eventDispatcher->addEventListenerWithSceneGraphPriority(e, this);
}

void RippleSprite::initGridSide()
{
    //----calculate nRow and nCol
    Size contentSize = this->getContentSize();
    _iRow = floorf(contentSize.height / _fGSLength) + 1;//add one more row to ensure buffer bigger than background pic
    _iCol = floorf(contentSize.width / _fGSLength) + 1;//add one more col to ensure buffer bigger than background pic
    // CCLOG("m_nRow,m_nCol:%i,%i",m_nRow,m_nCol);
    //----generate mesh
    //create mesh
    _pMesh = new Cmesh();
    _pMesh->autorelease();
    _pMesh->retain();
    //fill vertex attributes
    //nVertex should be (nRow+1)*(nCol+1)
    _pMesh->vlist.reserve((_iRow + 1) * (_iCol + 1));//in order to faster push_back, do reserve
    _pMesh->texCoordList.reserve((_iRow + 1) * (_iCol + 1));
    _pMesh->colorList.reserve((_iRow + 1) * (_iCol + 1));
    for(int i = 0; i < _iRow + 1; i++){
        for(int j = 0; j < _iCol + 1; j++){
            float x = _fGSLength * j;
            float y= contentSize.height - _fGSLength * i;
            float s = x / contentSize.width;
            float t = 1 - y / contentSize.height;
            Cv2 pos = Cv2(x, y);
            Cv2 texCoord = Cv2(s, t);
            Cv4 color = Cv4(1, 1, 1, 1);
            _pMesh->vlist.push_back(pos);
            _pMesh->texCoordList.push_back(texCoord);
            _pMesh->colorList.push_back(color);
        }
    }
    //fill indexs
    const int nVertexPerRow = _iCol + 1;
    for(int i = 0; i < _iRow; i++){
        for(int j = 0; j < _iCol; j++){
            int vID_LU = i * nVertexPerRow + j;
            int vID_RU = vID_LU + 1;
            int vID_LD = vID_LU + nVertexPerRow;
            int vID_RD = vID_LD + 1;
            CIDTriangle IDtri1 = CIDTriangle(vID_LU, vID_LD, vID_RD);
            CIDTriangle IDtri2 = CIDTriangle(vID_LU, vID_RD, vID_RU);
            _pMesh->IDtriList.push_back(IDtri1);
            _pMesh->IDtriList.push_back(IDtri2);
            
        }
    }
    //----m_texCoordList_store
    _vTexCoordListStore = _pMesh->texCoordList;
    //----create indexVBO
    _pIndexVBO = new CindexVBO();
    _pIndexVBO->autorelease();
    _pIndexVBO->retain();
    //----submit mesh
    _pIndexVBO->submitPos(_pMesh->vlist, GL_STATIC_DRAW);
    _pIndexVBO->submitTexCoord(_pMesh->texCoordList, GL_STATIC_DRAW);
    _pIndexVBO->submitColor(_pMesh->colorList, GL_STATIC_DRAW);
    _pIndexVBO->submitIndex(_pMesh->IDtriList, GL_STATIC_DRAW);
    
    //----generate srcBuffer and dstBuffer
    {
        //each element of srcBuffer or dstBuffer correspond to a vertex, present the vertex's height
        //so srcBuffer and dstBuffer are all (nRow+1)*(nCol+1) size
        //----srcBuffer
        _pSrcBuffer = new RippleBuffer();
        _pSrcBuffer->autorelease();
        _pSrcBuffer->retain();
        _pSrcBuffer->mat.resize(_iRow + 1);
        for(int i = 0; i < (int)_pSrcBuffer->mat.size(); i++) _pSrcBuffer->mat[i].resize(_iCol+1);
        //----dstBuffer
        _pDstBuffer = new RippleBuffer();
        _pDstBuffer->autorelease();
        _pDstBuffer->retain();
        *_pDstBuffer=*_pSrcBuffer;//m_dstBuffer copy m_srcBuffer
    }
    //change shader to have_mv
    setGLProgramState(GLProgramState::getOrCreateWithGLProgramName(GLProgram::SHADER_NAME_POSITION_TEXTURE_COLOR));
}

void RippleSprite::update(float dt)
{
    _fTime += dt;
    if(_fTime >= 1.0 / 45){
        updateOnce();
        _fTime = 0;
    }
}

void RippleSprite::updateOnce()
{
    //update buffer and mesh
    float k = 0.5 - 0.5 / RIPPLE_STRENGTH;
    float kTexCoord = 1.0 / 1048;
    const int nRow = _iRow + 1;
    const int nCol = _iCol + 1;
    for(int i = 1; i < nRow - 1; i++){
        for(int j = 1 ; j < nCol - 1; j++){
            //update m_dstBuffer
            float Hup_src = _pSrcBuffer->mat[i - 1][j];
            float Hdn_src = _pSrcBuffer->mat[i + 1][j];
            float Hleft_src = _pSrcBuffer->mat[i][j - 1];
            float Hright_src = _pSrcBuffer->mat[i][j + 1];
            float Hcenter_dst = _pDstBuffer->mat[i][j];
            float H=(Hup_src + Hdn_src + Hleft_src + Hright_src - 2 * Hcenter_dst) * k;
            _pDstBuffer->mat[i][j] = H;
            //update texCoord
            float s_offset = (Hup_src - Hdn_src) * kTexCoord;
            float t_offset = (Hleft_src - Hright_src) * kTexCoord;
            Cv2&texCoord = _pMesh->texCoordList[i * nCol + j];
            Cv2&texCoord_store =_vTexCoordListStore[i * nCol + j];
            texCoord.setx(texCoord_store.x() + s_offset);
            texCoord.sety(texCoord_store.y() + t_offset);
        }
    }
    RippleBuffer*temp= _pDstBuffer;
    _pDstBuffer = _pSrcBuffer;
    _pSrcBuffer = temp;
    //resubmit texCoord
    _pIndexVBO->submitTexCoord(_pMesh->texCoordList, GL_DYNAMIC_DRAW);
}

void RippleSprite::doTouch(const Vec2 &touchPoint,float depth, float r)
{
    //touchPoint is in parent space, convert it to local space
    this->getNodeToParentTransform();
    AffineTransform parentToNodeTransform = this->parentToNodeTransform();
    Vec2 touchPoint_localSpace = PointApplyAffineTransform(touchPoint, parentToNodeTransform);
    //convert touchPoint_localSpace to OLU (origin at left up corner) space
    Size contentSize = this->getContentSize();
    float x_OLU = touchPoint_localSpace.x;//origin at left up corner
    float y_OLU = contentSize.height-touchPoint_localSpace.y;//origin at left up corner
    //   CCLOG("x_OLU,y_OLU:%f,%f",x_OLU,y_OLU);
    //position range in OLU space
    float xmin = x_OLU-r;
    float xmax = x_OLU+r;
    float ymin = y_OLU-r;
    float ymax = y_OLU+r;
    //calculate index range from position range
    //note: min is floor(x), max is ceil ceil(x)-1
    int imin,imax,jmin,jmax;
    int nRow = _iRow + 1;
    int nCol = _iCol + 1;
    const int imargin = 1;//do not let the vertex on the edge to be pressed
    const int jmargin = 1;//do not let the vertex on the edge to be pressed
    imin = MAX(imargin, floorf(ymin / _fGSLength));
    imax = MIN(nRow - 1 - imargin, ceilf(ymax / _fGSLength) - 1);
    jmin = MAX(jmargin, floorf(xmin/_fGSLength));
    jmax = MIN(nCol - 1 - jmargin, ceilf(xmax / _fGSLength) - 1);
    //  cout<<"range:"<<imin<<" "<<imax<<" "<<jmin<<" "<<jmax<<endl;
    //iterate all vertex in range [imin,imax]x[jmin,jmax], and press them
    for(int i=imin;i<=imax;i++){
        for(int j=jmin;j<=jmax;j++){
            const Cv2&v = _pMesh->vlist[i*nCol+j];
            const Cv2&v_OLU=Cv2(v.x(),contentSize.height-v.y());
            //press m_srcBuffer at point v
            float dis = Vec2(x_OLU,y_OLU).getDistance(Vec2(v_OLU.x(),v_OLU.y()));
            if(dis < r){
                float curDepth = depth * (0.5 + 0.5 * cosf(dis * M_PI / r));
                _pSrcBuffer->mat[i][j] -= curDepth;
            }else{
                //do nothing;
            }
        }
    }
}
void RippleSprite::draw(Renderer* renderer, const Mat4 &transform, uint32_t flags)
{
    
    _customCommand.init(_globalZOrder);
    _customCommand.func = CC_CALLBACK_0(RippleSprite::onDraw,this,transform, flags);
    renderer->addCommand(&_customCommand);
    
}
void RippleSprite::onDraw(const Mat4 &transform, uint32_t flags)
{
    
    //use scissor test is more fast than clippingNode for large rect.
    //enable scissor test
    glEnable(GL_SCISSOR_TEST);
    //scissor test
    //calculate world rect
    Size contentSize = this->getContentSize();
    Rect rect_local = Rect(0,0,contentSize.width,contentSize.height);
    Rect rect_world = RectApplyAffineTransform(rect_local, this->getNodeToWorldAffineTransform());
    //do not use glScissor, instead use cocos2dx api CCEGLView::setScissorInPoints, or the result will be wrong.
    //note: setScissorInPoints receive world space rect as parameter
    Director::getInstance()->getOpenGLView()->setScissorInPoints(rect_world.getMinX(), rect_world.getMinY(), rect_world.size.width, rect_world.size.height);
    //----change shader
    GL::blendFunc( _blendFunc.src, _blendFunc.dst );
    //pass values for cocos2d-x build-in uniforms
    GLProgramUnifos *program=(GLProgramUnifos*)this->getGLProgram();
    program->use();
    //when program is have MV, we should use program->setUniformsForBuiltins(transform) instead of program->setUniformsForBuiltins()
    ////program->setUniformsForBuiltins();
    program->setUniformsForBuiltins(transform);
    //enable attributes
    bool isAttribPositionOn=CindexVBO::isEnabledAttribArray_position();
    bool isAttribColorOn=CindexVBO::isEnabledAttribArray_color();
    bool isAttribTexCoordOn=CindexVBO::isEnabledAttribArray_texCoord();
    CindexVBO::enableAttribArray_position(true);
    CindexVBO::enableAttribArray_color(true);
    CindexVBO::enableAttribArray_texCoord(true);
    //bindTexture
    GL::bindTexture2D( this->getTexture()->getName());
    _pIndexVBO->setPointer_position();
    _pIndexVBO->setPointer_texCoord();
    _pIndexVBO->setPointer_color();
    _pIndexVBO->draw(GL_TRIANGLES);
    GL::bindTexture2D(0);
    //disable attributes
    CindexVBO::enableAttribArray_position(isAttribPositionOn);
    CindexVBO::enableAttribArray_color(isAttribColorOn);
    CindexVBO::enableAttribArray_texCoord(isAttribTexCoordOn);
    //disable scissor test
    glDisable(GL_SCISSOR_TEST);
    
}

void RippleSprite::onEnter()
{
    Sprite::onEnter();
}

void RippleSprite::onExit()
{
    Sprite::onExit();
}