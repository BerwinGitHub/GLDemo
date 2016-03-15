//
//  DragSprite.hpp
//  Demo
//
//  Created by tangbowen on 8/3/16.
//
//

#ifndef DragSprite_hpp
#define DragSprite_hpp

#include "cocos2d.h"

USING_NS_CC;
using namespace std;

class DragSprite : public Sprite{
    
public:
    DragSprite();
    virtual ~DragSprite();
    
public:
    enum class EventType
    {
        ENTER,
        MOVED,
        EXIT
    };
    
    typedef struct _RectNode
    {
        Node *node;
        EventType type;
    }RectNode;
    
public:
    static DragSprite* createWithResponseNode(const string &file, Node *rNode);
    virtual bool initWithResponseNode(const string &file, Node *rNode);
    
public:
    CC_SYNTHESIZE(vector<RectNode>, _vResponseRectNodes, ResponseNode);
    CC_SYNTHESIZE(function<void(Sprite*, vector<RectNode> vRectNodes)>, _fnDelegate, Delegate);
    void addResponseNode(Node *n);
    void removeResponseNode(Node *n);
    void clearAllResponseNodes();
    
protected:
    /**
     * @brief 判断当前节点及其所有父节点的可见性
     */
    bool isAncestorsVisible(Node* node);
    bool onTouchBegan(Touch *t, Event *e);
    void onTouchMoved(Touch *t, Event *e);
    void onTouchEnded(Touch *t, Event *e);
    void onTouchCanceled(Touch *t, Event *e);
    
protected:
    virtual void onEnter() override;
    virtual void onExit() override;
    
private:
    /**
     * @brief 注册事件
     */
    void registerEvent();
    
    /**
     * @brief 判断当前节点是否在已经添加的节点(_vResponseNodes)里面
     */
    vector<RectNode> intersectsNodesRect(const Rect &rect);
    
    /**
     * @brief 将node的boundingBox转成世界坐标
     */
    Rect convertBoundingBoxToWorld(Node *n);
    
    /**
     * @brief 这个Node节点是否已经添加过了
     */
    bool isContainNode(Node *n);
    
    /**
     * @brief RectNode是否在已经在的节点集合内
     */
    bool isInExistRect(RectNode &rn, vector<RectNode> &v);
};

#endif /* DragSprite_hpp */
