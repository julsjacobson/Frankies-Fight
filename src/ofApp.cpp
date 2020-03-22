#include "ofApp.h"
#include "Emitter.h"


//----------------------------------------------------------------------------------
//
// This example code demonstrates the use of an "Emitter" to create a very
// minimal shooter video arcade game.  The game creates two(2) Emitters,
// one for the frankie (missiles launcher) and another one for emitting the
// squirrel.
//
// Game will run for 20 seconds.  To start a new game, press mouse button
//
//  Kevin M. Smith - CS 134 SJSU
//


//TODO: Animate emitters
//TODO: Move squirrel emitter in a sine wave
//TODO: Lose lives and then game over
//--------------------------------------------------------------
void ofApp::setup(){
    ofSetVerticalSync(true);
    //ofBackground(0, 0, 0);
    score = 0;
    
    
    // Record when the game starts - game will run for 10 sec
    //
    gameStartTime = ofGetElapsedTimeMillis();
    
    // Create and setup emitters  (you can play with parameters
    // I have another example program that creates a GUI for these
    // so with that you can get an idea of velocity/rate effects.
    //
    frankie = new Emitter( new SpriteSystem());
    squirrel = new Emitter(new SpriteSystem());
    cat = new Emitter(new SpriteSystem());




    
    //initialize background position
    grassPos = glm::vec3(0,0,0);
    grassPos2 = glm::vec3(ofGetWindowWidth(), 0,0);
    
    
    //Load sprite file
    //
    ofImage frankieImg;
    if (!frankieImg.load("images/frankieAnim.png")) {
        ofLogFatalError("can't load image: images/frankieAnim.png");
        ofExit();
    }
    frankie->setImage(frankieImg);
    frankie->setAnimImage(true);

    
   /* if (!frankie->image.load("images/frankieAnim.png")) {
        ofLogFatalError("can't load image: images/frankieAnim.png");
        ofExit();
    } else {
        frankie->image.load("images/frankieAnim.png");
        frankie->haveImage = true;
        frankie->setAnimImage(true);
    }*/
    if (!background.load("images/background.png")){
        ofLogFatalError("can't load image: images/background.png");
        ofExit();
    }
    else if(!grass.load("images/grass.png")) {
        ofLogFatalError("can't load image: images/grass.png");
        ofExit();
    }
    else if(!cloud.load("images/clouds.png")) {
        ofLogFatalError("can't load image: images/clouds.png");
        ofExit();
    }
    
    else {
        background.load("images/background.png");
        grass.load("images/grass.png");
        cloud.load("images/clouds.png");
    }
    
    
    //Create image for emitters, check if image is loaded
    if (!meow.load("images/meow.png")) {
        ofLogFatalError("can't load image: images/meow.png");
        ofExit();
    }
    else if (!woof.load("images/woof.png")) {
        ofLogFatalError("can't load image: images/woof.png");
        ofExit();
    }
    else if (!squirrelImg.load("images/squirrel.png")) {
        ofLogFatalError("can't load image: images/squirrel.png");
    }
    else if (!catImg.load("images/cat.png")) {
        ofLogFatalError("can't load image: images/cat.png");
        ofExit();
    }
    else{
        imageLoaded = true;
        meow.load("images/meow.png");
        meow.resize(60,20);
        woof.load("images/woof.png");
        woof.resize(56,56);
        catImg.load("images/cat.png");
        catImg.drawSubsection(500, 500, 128, 128, 0, 0);
        
        squirrelImg.load("images/squirrel.png");
    }
    
    
    frankie->setPosition(ofVec3f(10, ofGetWindowHeight()/2, 0));
    frankie->setVelocity(ofVec3f(1000, 0, 0));
    frankie->setRate(0);
    frankie->setLifespan(1000);
    frankie->setChildImage(woof);
    frankie->drawable = true;
    frankie->width = 128;
    frankie->height = 128;
    frankie->ntiles_x = 4;
    frankie->ntiles_y = 2;
    frankie->voff = 0;
    frankie->hoff = 0;
    frankie->nframes = 8;
    
    
    squirrel->setPosition(ofVec3f(ofGetWindowWidth() - 10, ofGetWindowHeight()/2, 0));
    squirrel->velocity.set(-400, 0, 0);
    squirrel->setLifespan(5000);
    squirrel->setRate(0);
    squirrel->setChildImage(squirrelImg);
    squirrel->drawable = false;

    
    cat->setPosition(ofVec3f(ofGetWindowWidth() - 10, ofGetWindowHeight()/2, 0));
    cat->velocity.set(-200,0,0);
    cat->setLifespan(7000);
    cat->setRate(.25);
    cat->setChildImage(catImg);
    cat->drawable = false;
    
    
    // start them up
    //
    cat->start();
    squirrel->start();
    frankie->start();

}

//--------------------------------------------------------------
//  ofApp::update() is called every frame by the OF mainloop.
//  If the frame rate is 60 fps (frames per second) it gets called
//  every 1/60 sec
//
void ofApp::update() {

    //When spacebar is pressed, frankie becomes animated
    //Background scrolls and missles fire when space bar is held down
    if (startGame) {
        squirrel->setRate(2);
        frankie->bAnimRunning = true;
        
        grassPos.x -= 250 * ofGetLastFrameTime();
        grassPos2.x -= (250 * ofGetLastFrameTime()) - ofGetWindowWidth();
        
        cloudPos.x -= 50 * ofGetLastFrameTime();
        cloudPos2.x -= (50 * ofGetLastFrameTime()) - ofGetWindowWidth();
        
        if (!bFire) frankie->setRate(0);
            else {
                frankie->setRate(4);
                //frankieWoof.play();
            }
    }
    
    //If frankie is not being moved with mouse, use mouse click to change direction
    //If the frankie IS being moved with the mouse, use default
    if (bDrag) {
        frankie->setVelocity(glm::vec3(1000, 0, 0));
    } else frankie->setVelocity(mouseLast - frankie->trans);

    frankie->update();
    squirrel->update();
    cat->update();

    // check for collisions between missles and squirrel
    checkCollisions();

    //Squirrel is emitted at random velocities
    ofVec3f v = squirrel->velocity;
    squirrel->setVelocity(ofVec3f(v.x,ofRandom(-v.x /2, 0), v.z));
    squirrel->trans.y = ofGetWindowHeight() * ofRandom(.3, 1);
    
    //Cat is emitted at random height in game
    cat->trans.y = ofGetWindowHeight() * ofRandom(.3, 1);
    
    
    //If run out of lives, game is over
    if (lives == 0) gameOver = true;
    
    

}


//--------------------------------------------------------------
//  ofApp::draw() this is where everything gets drawn
//  you should only put draw code in this method.
//
//
void ofApp::draw(){
    //Draw sky background
    background.draw(0,0,ofGetWindowWidth(),ofGetWindowHeight());

    //If game has not started, background is still and text is in the middle of the screen
    //Once game has started, animate background
    //TODO: grass background stops being seamless after awhile
    if (!startGame) {
        grass.draw(0,0,ofGetWindowWidth(), ofGetWindowHeight());
        cloud.draw(0,0,ofGetWindowWidth(), ofGetWindowHeight());
        ofDrawBitmapString("PRESS SPACE TO START", ofPoint(ofGetWindowWidth() / 2, ofGetWindowHeight() / 2));
    } else {
        grass.draw( int(grassPos.x) % ofGetWindowWidth(), grassPos.y, ofGetWindowWidth(), ofGetWindowHeight());
        grass.draw(int(grassPos2.x) % ofGetWindowWidth(), grassPos2.y, ofGetWindowWidth(), ofGetWindowHeight());
        
        cloud.draw(int(cloudPos.x) % ofGetWindowWidth(), cloudPos.y, ofGetWindowWidth(), ofGetWindowHeight());
        cloud.draw(int(cloudPos2.x) % ofGetWindowWidth(), cloudPos2.y, ofGetWindowWidth(), ofGetWindowHeight());
    }
    
    
    //Once game is over, draw "GAME OVER" text on screen
    if (gameOver) {
        ofDrawBitmapString("GAME OVER", ofPoint(ofGetWindowWidth() / 2, ofGetWindowHeight() / 2));
    }
    else {
        frankie->draw();
        squirrel->draw();
        cat->draw();
    }

    //Draw score
    string scoreText;
    string lifeText;
    scoreText += "Score: " + std::to_string(score);
    ofDrawBitmapString(scoreText, ofPoint(10, 20));
    lifeText += "Lives Remaining: " + std::to_string(lives);
    ofDrawBitmapString(lifeText, ofPoint(10, 50));
}

//  This is a simple O(M x N) collision check
//  For each missle check to see which squirrel you hit and remove them
//
void ofApp::checkCollisions() {

    // find the distance at which the two sprites (missles and squirrel) will collide
    // detect a collision when we are within that distance.
    //
    float collisionDist = frankie->childWidth / 2 + squirrel->childWidth / 2;
    float collisionDist2 = frankie->childWidth / 2 + cat->childWidth / 2;
    
    float collisionFrankie = frankie->height/2 + squirrel->childWidth/2;

    // Loop through all the missiles, then remove any squirrel that are within
    // "collisionDist" of the missiles.  the removeNear() function returns the
    // number of missiles removed.
    //
    for (int i = 0; i < frankie->sys->sprites.size(); i++) {
        score += squirrel->sys->removeNear(frankie->sys->sprites[i].trans, collisionDist);
        score += cat->sys->removeNear(frankie->sys->sprites[i].trans, collisionDist2);
    }
    
    
}

void ofApp::mouseMoved(int x, int y) {
    glm::vec3 mouse(x,y,0);
    if (((frankie->trans[0] - 96) <= mouse[0]) && (mouse[0] <= (frankie->trans[0] + 96)) && ((frankie->trans[1] - 96) <= mouse[1]) && (mouse[1] <= (frankie->trans[1] + 96)))
        inside = true;
    else inside = false;
}

//  support dragging of frankie with mouse
void ofApp::mouseDragged(int x, int y, int button){
    if (!bDrag) return;
    ofPoint mouse_cur = ofPoint(x, y);
    ofVec3f delta = mouse_cur - mouseLast;

    //Allow frankie to be moved where grass is and stay inside window
    if (inside && (frankie->trans.y + delta.y) > ofGetWindowHeight() *.3  && (frankie->trans.y + delta.y) >= 0
        && (frankie->trans.x + delta.x) < ofGetWindowWidth() && (frankie->trans.x + delta.x) >= 0) {
        frankie->trans += delta;
        mouseLast = mouse_cur;
    }
}

//--------------------------------------------------------------
// Start game if not running; otherwise track mouse last coord
//
void ofApp::mousePressed(int x, int y, int button){
    bDrag = true;
    if (!frankie->started) {
        gameStartTime = ofGetElapsedTimeMillis();
        gameOver = false;
        frankie->start();
        
        cat->start();
        squirrel->start();
        score = 0;
    }
    mouseLast = ofPoint(x, y);
}

void ofApp::mouseReleased(int x, int y, int button) {
    bDrag = false;
}

void ofApp::keyPressed(int key) {
    switch (key) {
    case 'C':
    case 'c':
        break;
    case 'F':
    case 'f':
        ofToggleFullscreen();
        break;
    case 'H':
    case 'h':
        bHide = !bHide;
        break;
    case 'r':
        break;
    case 's':
        break;
    case 'u':
        break;

            
    case ' ':
        bFire = true;
        startGame = true;
        break;
            
    //Controll using arrow keys
    case OF_KEY_UP:
        frankie->trans[1] -= 10;
        break;
    case OF_KEY_DOWN:
        frankie->trans[1] += 10;
        break;
    case OF_KEY_LEFT:
        frankie->trans[0] -= 10;
        break;
    case OF_KEY_RIGHT:
        frankie->trans[0] += 10;
        break;

    case OF_KEY_ALT:
        break;
    case OF_KEY_CONTROL:
        break;
    case OF_KEY_SHIFT:
        break;
    case OF_KEY_DEL:
        break;
            
    }
}


//--------------------------------------------------------------
void ofApp::keyReleased(int key) {
    switch (key) {
        case ' ':
            bFire = false;
    case OF_KEY_LEFT:
    case OF_KEY_RIGHT:
    case OF_KEY_UP:
    case OF_KEY_DOWN:
        break;
    case OF_KEY_ALT:
        break;
    case OF_KEY_CONTROL:
        break;
    case OF_KEY_SHIFT:
        break;
    }
}
