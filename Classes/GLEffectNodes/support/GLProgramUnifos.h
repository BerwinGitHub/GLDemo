//
//  GLProgramWithUnifos.h
//  Demo
//
//  Created by yang tangbowen on 11/3/16.
//
//

#ifndef __Demo__GLProgramWithUnifos__
#define __Demo__GLProgramWithUnifos__

#include <iostream>
#include<map>
#include "cocos2d.h"

USING_NS_CC;
using namespace std;

class GLProgramUnifos : public GLProgram {
public:
    GLProgramUnifos(){}
	virtual ~GLProgramUnifos(){}
    
public:
    void attachUniform(string unifoName){
        m_unifoMap[unifoName] = glGetUniformLocation(this->getProgram(),unifoName.c_str());
    }
    void passUnifoValue1f(string unifoName,GLfloat v);
    void passUnifoValue1i(string unifoName,GLint v);
    void passUnifoValueMatrixNfv(string unifoName,const GLfloat *array,int arrayLen);
    void passUnifoValueNfv(string unifoName,const GLfloat *array,int arrayLen);
    void passUnifoValueNfvN(string unifoName,const GLfloat*array,int arrayLen,int arrayCount);
    void passUnifoValueNiv(string unifoName,const GLint *array,int arrayLen);
    void passUnifoValueMatrixNfv(string unifoName,const vector<GLfloat>&valueList);
    void passUnifoValueNfv(string unifoName,const vector<GLfloat>&valueList);
    void passUnifoValueNiv(string unifoName,const vector<GLint>&valueList);
    
protected:
    map<string,GLint> m_unifoMap;
    
};
#endif /* defined(__Demo__GLProgramWithUnifos__) */
