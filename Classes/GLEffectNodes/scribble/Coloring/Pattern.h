//
//  Pattern.h
//  ScribbleDemo_3_2
//
//  Created by renhong on 8/17/15.
//
//

#ifndef __ScribbleDemo_3_2__Pattern__
#define __ScribbleDemo_3_2__Pattern__

#include <stdio.h>
#include "BoundaryFillNode.h"
class Pattern;
class PatternLoadDelegate{
public:
    virtual void loadPercent(Pattern *pNode, float pPercent) = 0;//Pattern加载进度0.0~1.0
    virtual void loadEnded(Pattern *pNode) = 0;
};

class Pattern : public BoundaryFillNode {
public:
    static Pattern *create(string pLineFrameFile);
    virtual bool init(string pLineFrameFile);
    Pattern();
    ~Pattern();
    void fillPatternAsync();
    void fillPattern();
    void refreshPatternData();
    void loadPercent(float pTime);
    
    inline void setDelegate(PatternLoadDelegate *pDelegate){_patternLoadDelegate = pDelegate;}
    inline PatternLoadDelegate *getDelegate(){return _patternLoadDelegate;}
    Image *getPatternImage();
    static inline void setPatternColorStep(int pStep = 5){_patternColorStep = pStep;}
    bool savePattern(string pPath);
    inline static Color3B getPatternStartColor(){return _patternStartColor;}
private:
    static Color3B _patternStartColor;
    static Color3B _patternEndColor;
    static int _patternColorStep;
    vector<Color4B> _colors;
    int _colorCounter;
    
    float _patternLoadPercent;
    pthread_mutex_t _mutex;
    bool _async;
    
    PatternLoadDelegate *_patternLoadDelegate;
};
#endif /* defined(__ScribbleDemo_3_2__Pattern__) */
