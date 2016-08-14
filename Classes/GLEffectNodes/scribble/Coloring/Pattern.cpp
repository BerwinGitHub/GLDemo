//
//  Pattern.cpp
//  ScribbleDemo_3_2
//
//  Created by renhong on 8/17/15.
//
//

#include "Pattern.h"
#include "ScribbleUtil.h"
int Pattern::_patternColorStep = 5;
Color3B Pattern::_patternStartColor(10, 10, 10);
Color3B Pattern::_patternEndColor(250, 250, 250);
Pattern *Pattern::create(string pLineFrameFile){
    Pattern *ret = new Pattern();
    if(ret && ret->init(pLineFrameFile)){
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

Pattern::Pattern(){
    _colorCounter = 0;
    _async = false;
    pthread_mutex_init(&_mutex, nullptr);
    for (int b = _patternStartColor.b; b <= _patternEndColor.b; b += _patternColorStep) {
        for (int g = _patternStartColor.g; g <= _patternEndColor.b; g += _patternColorStep) {
            for (int r = _patternStartColor.r; r <= _patternEndColor.b; r += _patternColorStep) {
                _colors.push_back(Color4B(r, g, b, 255));
            }
        }
    }
}

Pattern::~Pattern(){
    pthread_mutex_destroy(&_mutex);
    this->unschedule(schedule_selector(Pattern::loadPercent));
    log("Pattern::~Pattern()");
}

bool Pattern::init(string pLineFrameFile){
    if (!BoundaryFillNode::init(pLineFrameFile)) {
        return false;
    }
    
    this->schedule(schedule_selector(Pattern::loadPercent));
    return true;
}

void Pattern::fillPatternAsync(){
    _async = true;
    thread lThread = std::thread(&Pattern::fillPattern,this);
    lThread.detach();
}

void Pattern::fillPattern(){
    if (_async) {
        pthread_mutex_lock(&_mutex);
    }
    _colorCounter = 0;
    
    log("start      %ld %ld", ScribbleUtil::getInstance()->getCurrentTimeByMicroseconds(), time(nullptr));
    for (int r = 1; r < this->getSize().height; ++r) {
        for (int c = 1; c < this->getSize().width; ++c) {
            Color4B lColor = this->getPixelColor(c, this->getSize().height - r);
            if (lColor.a <= 0) {
                Color4B lFillColor = _colors.at(_colorCounter);
                ++_colorCounter;
                //    log("lFillColor %d %d %d", lFillColor.r, lFillColor.g, lFillColor.b);
                
                //                log("pre fill %d: %d %d", _colorCounter, c, r);
                this->fill(lFillColor, this->convertToWorldSpace(Vec2(c, r)));
            }
        }
        _patternLoadPercent = ((float)r) / this->getSize().height;
    }
    log("pre fill number %d", _colorCounter);
    log("middle     %ld %ld", ScribbleUtil::getInstance()->getCurrentTimeByMicroseconds(), time(nullptr));
    if (_async) {
        pthread_mutex_unlock(&_mutex);
    }

    this->scheduleOnce(SEL_SCHEDULE(&Pattern::refreshPatternData), 0);
}

Image *Pattern::getPatternImage(){
    Image *lImage = new Image();
    lImage->initWithRawData(_imageData, (int)this->getSize().width * this->getSize().height * 4, (int)this->getSize().width, (int)this->getSize().height, 8);
    lImage->autorelease();
    return lImage;
}

bool Pattern::savePattern(string pPath){
    Image *lPatternImage = this->getPatternImage();
    return lPatternImage->saveToFile(pPath);
}

void Pattern::loadPercent(float pTime){
    if (nullptr != this->getDelegate()) {
        this->getDelegate()->loadPercent(this, _patternLoadPercent);
    }
}

void Pattern::refreshPatternData(){
    this->updateImage();
    if (nullptr != this->getDelegate()) {
        this->getDelegate()->loadEnded(this);
    }
}