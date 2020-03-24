#pragma once
//----------------------------------------------------------------------------------
//
// Basic Sprite Class
//
//
//  Kevin M. Smith - CS 134 SJSU

#include "ofMain.h"

// This is a base object that all drawable object inherit from
// It is possible this will be replaced by ofNode when we move to 3D
//
class BaseObject {
public:
    BaseObject();
    ofVec2f trans, scale;
    float    rot;
    bool    bSelected;
    void setPosition(ofVec3f);
};

//  General Sprite class  (similar to a Particle)
//
class Sprite : public BaseObject {
public:
    Sprite();
    void drawSub();
    void draw();
    float age();
    void setImage(ofImage);
    void startAnim();
    void stopAnim();
    void advanceFrame();
    void update();
    void setAnimImage(bool f) { haveAnimImage = f; }
    


    
    
    float speed;    //   in pixels/sec
    ofVec3f velocity; // in pixels/sec
    ofImage image;
    float birthtime; // elapsed time in ms
    float lifespan;  //  time in ms
    string name;
    bool haveImage;
    float width, height;
    

    
    bool drawsub;
    
    
    bool haveAnimImage = false;
    //float width, height;
    glm::vec3 pos;
    float voff, hoff;
    int ntiles_x, ntiles_y, nframes;
    int frame = 0;
    int row = 0;
    int col = 0;
    bool bAnimRunning = false;
    float lastTimeRec;
    
    float xposition = ofGetWindowWidth();
    
    
};

//  Manages all Sprites in a system.  You can create multiple systems
//
class SpriteSystem {
public:
    void add(Sprite);
    void remove(int);
    void update();
    int removeNear(ofVec3f point, float dist);
    
        
    int loseALife(ofVec3f point, float dist);
    
    void draw();
    vector<Sprite> sprites;

    ofSoundPlayer hit;
    
    
    
    glm::vec3 curveEval(float x, float scale, float cycles, float yposition);

    bool bwave = false;
    float amplitude, cycles;
    float yposition; 
};
