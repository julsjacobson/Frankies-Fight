#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "Sprite.h"
#include "ParticleSystem.h"
#include "ParticleEmitter.h"

typedef enum { MoveStop, MoveLeft, MoveRight, MoveUp, MoveDown } MoveDir;



class Emitter;

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
		void checkCollisions();

    void keyPressed(int);
    void keyReleased(int);
    void mouseMoved(int x, int y);
    void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    
		void mouseEntered(int x, int y) {}
		void mouseExited(int x, int y) {}
		void windowResized(int w, int h) {}
		void dragEvent(ofDragInfo dragInfo) {}
		void gotMessage(ofMessage msg) {}
    //void checkLife();
    void loseALife(Emitter *e, float dist);
    void gainALife(Emitter*e, float dist); 
   
    void explosion(Sprite *s); 
		Emitter *squirrel, *frankie, *cat;
    Emitter *mailman, *firehydrant;
    Emitter *bone; 
		int score;
    int level = 1; 
    int lives = 5;
    float gameStartTime;
    bool gameOver = false;
		ofVec3f mouseLast;
    bool startGame; 
    bool bFire;
    bool bHide;
    bool imageLoaded;
    
     ofImage startText;
    ofImage background;
    ofImage grass;
    ofImage cloud;
    glm::vec3 grassPos;
    glm::vec3 grassPos2;
    glm::vec3 cloudPos;
    glm::vec3 cloudPos2;
    
    ofSoundPlayer shoot;
    
    /*Music: Eric Skiff - Chibi Ninja - Resistor Anthems - Available at http://EricSkiff.com/music */
    /* Music: Eric Skiff - Prologue - Resistor Anthems - Available at http://EricSkiff.com/music */
    ofSoundPlayer chibiNinja;
    ofSoundPlayer prologue; 
    
    ofImage meow, squirrelImage, woof, squirrelImg, catImg;
    ofImage mailmanImg, smokeImg;
    ofImage fireImg;
    
    
    bool bDrag = false;
    bool inside;
    float xposition = ofGetWindowWidth();
    

    ParticleEmitter water;
    ParticleEmitter mail;
    
    ofImage mailImg;
    ofImage waterImg;
    ofImage boneImg;
    
    ofSoundPlayer lose;
    ofSoundPlayer pickup; 
};


