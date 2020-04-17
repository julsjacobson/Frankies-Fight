#include "Sprite.h"

BaseObject::BaseObject() {
    trans = ofVec3f(0,0,0);
    scale = ofVec3f(1, 1, 1);
    rot = 0;
}

void BaseObject::setPosition(ofVec3f pos) {
    trans = pos;
}

//
// Basic Sprite Object
//
Sprite::Sprite() {
    speed = 0;
    velocity = ofVec3f(0, 0, 0);
    lifespan = -1;      // lifespan of -1 => immortal
    birthtime = 0;
    bSelected = false;
    haveImage = false;
    name = "UnamedSprite";
    width = 20;
    height = 20;
}

// Return a sprite's age in milliseconds
//
float Sprite::age() {
    return (ofGetElapsedTimeMillis() - birthtime);
}

//  Set an image for the sprite. If you don't set one, a rectangle
//  gets drawn.
//
void Sprite::setImage(ofImage img) {
    image = img;
    haveImage = true;
}


//  Render the sprite
//
void Sprite::draw() {

    ofSetColor(255, 255, 255, 255);

    // draw image centered and add in translation amount
    //
  
    if (haveImage && haveAnimImage) {
        image.drawSubsection(trans.x + pos.x, trans.y +pos.y, width, height, col*width+hoff, row*height+voff, width, height);
    
    }
    else if (haveImage) {
        image.draw(image.getWidth() / 2.0 + trans.x, image.getHeight() / 2.0 + trans.y);
    }
    else {
        ofDrawRectangle(-width / 2 + trans.x, -height / 2 + trans.y, width, height);
    }

}

void Sprite::startAnim() {
    frame = 0;
    bAnimRunning = true;
    lastTimeRec = ofGetSystemTimeMillis();
}

void Sprite::stopAnim() {
    bAnimRunning = false;
}


void Sprite::advanceFrame() {
    if (frame == (nframes - 1)) {
        col = 0;
        row = 0;
        frame = 0;  // cycle back to first frame
    }
    else {
        frame++;
        if (col == ntiles_x - 1) col = 0; else col++;
        row = frame / ntiles_x;
    }
}

void Sprite::update() {
    if (bAnimRunning && haveAnimImage) {

        float curTime = ofGetSystemTimeMillis();
        if ((curTime - lastTimeRec) > 50) {
            advanceFrame();
            lastTimeRec = curTime;
        }
    }
    

    
}




//  Add a Sprite to the Sprite System
//
void SpriteSystem::add(Sprite s) {
    sprites.push_back(s);
}

// Remove a sprite from the sprite system. Note that this function is not currently
// used. The typical case is that sprites automatically get removed when the reach
// their lifespan.
//
void SpriteSystem::remove(int i) {
    sprites.erase(sprites.begin() + i);
}

// remove all sprites within a given dist of point, return number removed
//
int SpriteSystem::removeNear(ofVec3f point, float dist) {
    if (!hit.load("sounds/hit.wav")) {
        ofLogFatalError("can't load sound: images/hit.wav");
        ofExit();
    }
    hit.setMultiPlay(true);
    
    vector<Sprite>::iterator s = sprites.begin();
    vector<Sprite>::iterator tmp;
    int count = 0;

    while (s != sprites.end()) {
        ofVec3f v = s->trans - point;
        
        lastPos = s->trans;
        if (v.length() < dist) {
            tmp = sprites.erase(s);
            count+= points;
            
            hit.play();
            s = tmp;
            

        }
        else
        {
            lastPos = ofVec3f(0,0,0);
            s++;
        }
    }
    return count;
}



//  Update the SpriteSystem by checking which sprites have exceeded their
//  lifespan (and deleting).  Also the sprite is moved to it's next
//  location based on velocity and direction.
//
void SpriteSystem::update() {
    


    if (sprites.size() == 0) return;
    vector<Sprite>::iterator s = sprites.begin();
    vector<Sprite>::iterator tmp;

    // check which sprites have exceed their lifespan and delete
    // from list.  When deleting multiple objects from a vector while
    // traversing at the same time, use an iterator.
    //
    while (s != sprites.end()) {
        if (s->lifespan != -1 && s->age() > s->lifespan) {
            //            cout << "deleting sprite: " << s->name << endl;
            tmp = sprites.erase(s);
            s = tmp;
        }
        else s++;
    }

    //  Move sprite
        for (int i = 0; i < sprites.size(); i++) {
    
            
            if (bwave) {
                sprites[i].trans = curveEval(sprites[i].xposition, amplitude, cycles, yposition);
                
            
            } else sprites[i].trans += sprites[i].velocity / ofGetFrameRate();
        }
    
}

//  Render all the sprites
//
void SpriteSystem::draw() {
    for (int i = 0; i < sprites.size(); i++) {
        sprites[i].draw();
    }
}


glm::vec3 SpriteSystem::curveEval(float x, float scale, float cycles, float yposition)
{
    // x is in screen coordinates and his in [0, WindowWidth]
    float u = (cycles * x * PI) / ofGetWidth();
    return (glm::vec3(x, -scale *sin(u) + yposition, 0));
}
