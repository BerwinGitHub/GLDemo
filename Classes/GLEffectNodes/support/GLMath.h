//
//  Math.h
//  GLDemo
//
//  Created by yang tangbowen on 11/3/16.
//
//

#ifndef __GLDemo__Math__
#define __GLDemo__Math__

#include <iostream>
#include "cocos2d.h"

USING_NS_CC;
using namespace std;

float random01();


float randomInRange( float min, float max );


bool catchProbability( float p );


bool isPointEqual( const Vec2 &point1, const Vec2 &point2, float eps );


Vec2 rotateVector2( const Vec2 &vec, float A );


Vec2 rotateVector2( const Vec2 &vec, float cosA, float sinA );


float calculateAngleFromCosAndSin( float cosA, float sinA );


Vec2 calculateCosASinAOfVec1ToVec2( const Vec2 &vec1, const Vec2 &vec2 );


float nPow( float a, int n );


float pow2( float x );


float PNDisPointToLine( const Vec2 &point, const Vec2 &start, const Vec2 &end );


bool isPointEqual( const Vec2 &p1, const Vec2 &p2 );


Vec2 getIntersectPointOfTwoLine( const Vec2 &p1, const Vec2 &dir1, const Vec2 &p2, const Vec2 &dir2 );


float calculateIncircleR2( const Vec2 &v0, const Vec2 &v1, const Vec2 &v2 );


vector<Vec2> makeTriangleSmaller( Vec2 v0, Vec2 v1, Vec2 v2, float d );


Vec2 getRightNormal( const Vec2 &dir );


Vec2 getRightVector( const Vec2 &p1, const Vec2 &p2 );


float calculateTriangleArea( const Vec2 &v0, const Vec2 &v1, const Vec2 &v2 );


vector<Vec2> calculateTangentPointsOfPointToCircle( const Vec2 &center, float r, const Vec2 &point );


Vec2 M4TransV2( const Mat4 &mat, const Vec2 &v );


#endif /* defined(__GLDemo__Math__) */
