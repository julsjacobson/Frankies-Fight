#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "Sprite.h"

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
		
		Emitter *squirrel, *frankie, *cat;
		int score;
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
    
    
    ofImage meow, squirrelImage, woof, squirrelImg, catImg;
    
    bool bDrag = false;
    bool inside;
};

