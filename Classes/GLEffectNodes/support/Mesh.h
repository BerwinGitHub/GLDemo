//
//  Mesh.h
//  GLDemo
//
//  Created by yang tangbowen on 11/3/16.
//
//

#ifndef __GLDemo__Mesh__
#define __GLDemo__Mesh__

#include <iostream>
#include <vector>
#include "cocos2d.h"
#include "GLClasses.h"

using namespace std;
using namespace cocos2d;

class Cmesh : public Ref
{
public:
    vector<Cv2>		vlist;
    vector<Cv2>		texCoordList;
    vector<Cv4>		colorList;
    vector<CIDTriangle>	IDtriList;
    void clear()
    {
        vlist.clear();
        texCoordList.clear();
        colorList.clear();
        IDtriList.clear();
    }
};

class CmeshWire : public Ref
{
public:
    vector<Cv2>	vlist;
    vector<Cv2>	texCoordList;
    vector<Cv4>	colorList;
    vector<CIDLine> IDlineList;
    void clear()
    {
        vlist.clear();
        texCoordList.clear();
        colorList.clear();
        IDlineList.clear();
    }
};

class CmeshGeneral : public Ref
{
public:
    vector<Cv2>	vlist;
    vector<Cv2>	texCoordList;
    vector<Cv4>	colorList;
    vector<short>	indexList;
    void clear()
    {
        vlist.clear();
        texCoordList.clear();
        colorList.clear();
        indexList.clear();
    }
};
#endif /* defined(__GLDemo__Mesh__) */
