//
//  ScribbleWireframeNode.h
//  ScribbleDemo_3_2
//
//  Created by renhong on 4/2/15.
//
//

#ifndef __ScribbleDemo_3_2__ScribbleWireframeNode__
#define __ScribbleDemo_3_2__ScribbleWireframeNode__

#include <stdio.h>
#include "ScribblePatternNode.h"

class ScribbleWireframeNode : public ScribblePatternNode {
public:
    static ScribbleWireframeNode *create(Size pCanvasSize);
    void setWireframe(Node *pWireframe, Vec2 pPosition = Vec2::ZERO);
    Node *getWireframe(){return _wireframe;}
    Color4B getPatternColor(Vec2 pWorldPosition);
    void setPatternColor(Vec2 pWorldPosition);
private:
    Node *_wireframe;
};
#endif /* defined(__ScribbleDemo_3_2__ScribbleWireframeNode__) */
