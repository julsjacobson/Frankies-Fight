#include "ofMain.h"
#include "Emitter.h"



//  Create a new Emitter - needs a SpriteSystem
//
Emitter::Emitter(SpriteSystem *spriteSys) {
    sys = spriteSys;
    lifespan = 3000;    // milliseconds
    started = false;

    lastSpawned = 0;
    rate = 1;    // sprites/sec
    haveChildImage = false;
    haveImage = false;
    haveAnimImage = false;
    velocity = ofVec3f(100, 100, 0);
    drawable = true;
    width = 50;
    height = 50;
    childWidth = 10;
    childHeight = 10;
}

//  Draw the Emitter if it is drawable. In many cases you would want a hidden emitter
//
//
void Emitter::draw() {
    //draw sprite system
    sys->draw();
    if (drawable) {

        if (haveImage && haveAnimImage) {
            image.drawSubsection(trans.x + pos.x, trans.y +pos.y, width, height, col*width+hoff, row*height+voff, width, height);
        }
        else if (haveImage) {
            image.draw(-image.getWidth() / 2.0 + trans.x, -image.getHeight() / 2.0 + trans.y);
        }
        else {
            ofSetColor(0, 0, 0);
            ofDrawRectangle(-width / 2 + trans.x, -height / 2 + trans.y, width, height);
        }
    }

}


void Emitter::update() {
    
    // update animated image on emitter (if there is one)
    //
    if (bAnimRunning && haveAnimImage) {

        float curTime = ofGetSystemTimeMillis();
        if ((curTime - lastTimeRec) > 50) {
            advanceFrame();
            lastTimeRec = curTime;
        }
    }

    // update sprite emission (if the emitter is "started")
    //
    if (started) {
        float time = ofGetElapsedTimeMillis();
        if ((time - lastSpawned) > (1000.0 / rate)) {
            // spawn a new sprite
            Sprite sprite;
            if (haveChildImage) sprite.setImage(childImage);
            sprite.velocity = velocity;
            sprite.lifespan = lifespan;
            sprite.setPosition(trans);
            sprite.birthtime = time;
            sprite.width = childWidth;
            sprite.height = childHeight;
            sys->add(sprite);
            lastSpawned = time;
        }
        sys->update();
    }
}

// Start/Stop the emitter.
//
void Emitter::start() {
    started = true;
    lastSpawned = ofGetElapsedTimeMillis();
}

void Emitter::stop() {
    started = false;
}


void Emitter::setLifespan(float life) {
    lifespan = life;
}

void Emitter::setVelocity(ofVec3f v) {
    velocity = v;
}

void Emitter::setChildImage(ofImage img) {
    childImage = img;
    haveChildImage = true;
    childWidth = img.getWidth();
    childHeight = img.getHeight();
}

void Emitter::setImage(ofImage img) {
    image = img;
    haveImage = true;
}

void Emitter::setRate(float r) {
    rate = r;
}

float Emitter::maxDistPerFrame() {
    return  velocity.length() / ofGetFrameRate();
}

void Emitter::startAnim() {
    frame = 0;
    bAnimRunning = true;
    lastTimeRec = ofGetSystemTimeMillis();
}

void Emitter::stopAnim() {
    bAnimRunning = false;
}


void Emitter::advanceFrame() {
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
