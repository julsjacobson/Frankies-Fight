#pragma once
//----------------------------------------------------------------------------------
//
// Basic Sprite Emitter Class
//
//
//  Kevin M. Smith - CS 134 SJSU

#include "Sprite.h"


//  General purpose Emitter class for emitting sprites
//  This works similar to a Particle emitter
//
class Emitter : public BaseObject {
public:
    Emitter(SpriteSystem *);
    void draw();
    void start();
    void stop();
    void setLifespan(float);    // in milliseconds
    void setVelocity(ofVec3f);  // pixel/sec
    void setChildImage(ofImage);
    void setChildSize(float w, float h) { childWidth = w; childHeight = h; }
    void setImage(ofImage);
    void setAnimImage(bool f) { haveAnimImage = f; }
    void setRate(float);
    
    void startAnim();
    void stopAnim();
    void advanceFrame();
    
    float maxDistPerFrame();
    void update();
    SpriteSystem *sys;
    float rate;
    ofVec3f velocity;
    float lifespan;
    bool started;
    float lastSpawned;
    ofImage childImage;
    ofImage image;
    bool drawable;
    bool haveChildImage;
    bool haveImage;
    bool haveAnimImage = false;
    float width, height;
    float childWidth, childHeight;
    
    glm::vec3 pos;
    float voff;
    float hoff;
    
    int ntiles_x, ntiles_y;
    int nframes;
    int frame = 0;
    int row = 0;
    int col = 0;
    bool bAnimRunning = false;

    float lastTimeRec;
};
