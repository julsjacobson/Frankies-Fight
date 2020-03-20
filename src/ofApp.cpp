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
	frankie = new Emitter( new SpriteSystem() );
	squirrel = new Emitter(new SpriteSystem());
    cat = new Emitter(new SpriteSystem());




    
    //initialize background position
    grassPos = glm::vec3(0,0,0);
    grassPos2 = glm::vec3(ofGetWindowWidth(), 0,0);
    
    
    //Load sprite file
    
    if (!frankie->image.load("images/Frankie.png")) {
        ofLogFatalError("can't load image: images/Frankie.png");
        ofExit();
    } else {
        frankie->image.load("images/Frankie.png");
        frankie->haveImage = true;
    }
    if (!background.load("images/background.png")){
        ofLogFatalError("can't load image: images/background.png");
        ofExit();
    }
    else if (!startText.load("images/start.png")) {
        ofLogFatalError("can't load image: images/start.png");
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
        startText.load("images/start.png");
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
    else if (!catImg.load("images/cat.gif")) {
        ofLogFatalError("can't load image: images/cat.gif");
        ofExit();
    }
    else{
        imageLoaded = true;
        meow.load("images/meow.png");
        meow.resize(60,20);
        woof.load("images/woof.png");
        woof.resize(56,56);
        catImg.load("images/cat.gif");
        catImg.drawSubsection(500, 500, 128, 128, 0, 0); 
        
        squirrelImg.load("images/squirrel.png");
    }
    
    
    
    // Set up some reasonable parameters for the frankie/missile launcher
    // Remember that "Y" decreases as we move toward teh top of the
    // window, so we set a negative velocity of -1000;
    // We also don't want the missiles to live forever, so we set
    // a lifespan of 500 ms.
    //
    frankie->setPosition(ofVec3f(10, ofGetWindowHeight()/2, 0));
    frankie->setVelocity(ofVec3f(1000, 0, 0));
    frankie->setRate(3);
    frankie->setLifespan(1000);
    frankie->setChildImage(woof);
    frankie->drawable = true;
    
    
	// Set up some reasonable parameters for the invader spirtes
	// Remember that "Y" decreases as we move toward the bottom of
	// the window, so we set a negative velocity of 400.
	// We also don't want the squirrel to live forever, so we set
	// a lifespan of 5000 ms.  they are traveling much slower
    // than the missiles, so they need to live longer
	//
	squirrel->setPosition(ofVec3f(ofGetWindowWidth() - 10, ofGetWindowHeight()/2, 0));
	squirrel->velocity.set(-400, 0, 0);
	squirrel->setLifespan(5000);
	squirrel->setRate(2);
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

	//  call the emitter's update function to create new sprites
	//  as needed
	//
	frankie->update();
	squirrel->update();
    cat->update();

	// check for collisions between missles and squirrel
	//  
	checkCollisions();

	// we will randomize initial velocity so that not the squirrel
	// are going at the same speed just to make it a little harder/interesting
	// to play
	//
	ofVec3f v = squirrel->velocity;
	squirrel->setVelocity(ofVec3f(v.x,ofRandom(-v.x /2, 0), v.z));
    squirrel->trans.y = ofGetWindowHeight() * ofRandom(.3, 1);
    
    cat->trans.y = ofGetWindowHeight() * ofRandom(.3, 1);
    
    
    /*
    setPosition(ofVec3f(ofGetWindowWidth(), ofGetWindowHeight() * ofRandom(1, 10)), 0);
*/
    
	// game runs for 20 seconds; check to see if over and stop
	// emitters.  We are computing in milleseconds so we need
	// to multiply seconds by 1000 to convert to ms.
	//
    /*
	float t = ofGetElapsedTimeMillis();
	if ((t - gameStartTime > (20 * 1000))) {
		gameOver = true;
		frankie->stop();
		squirrel->stop();
        cat->stop();
	}*/
    
    if (lives == 0) gameOver = true;
    
            grassPos.x -= 250 * ofGetLastFrameTime();
        grassPos2.x -= (250 * ofGetLastFrameTime()) - ofGetWindowWidth();
        
        cloudPos.x -= 50 * ofGetLastFrameTime();
        cloudPos2.x -= (50 * ofGetLastFrameTime()) - ofGetWindowWidth();

}


//--------------------------------------------------------------
//  ofApp::draw() this is where everything gets drawn
//  you should only put draw code in this method. 
//
//
void ofApp::draw(){
        background.draw(0,0,ofGetWindowWidth(),ofGetWindowHeight());

        grass.draw(0,0,ofGetWindowWidth(), ofGetWindowHeight());
        cloud.draw(0,0,ofGetWindowWidth(), ofGetWindowHeight());
    
    
	// if game is over, just draw a label in middle of screen
	//
	if (gameOver) {
		ofDrawBitmapString("GAME OVER", ofPoint(ofGetWindowWidth() / 2, ofGetWindowHeight() / 2));
	}
	else {
		frankie->draw();
		squirrel->draw();
        cat->draw();
	}

	// draw current score
	//
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
	float collisionDist = frankie->childHeight / 2 + squirrel->childHeight / 2;
    float collisionDist2 = frankie->childHeight / 2 + cat->childHeight / 2;
    
    float collisionFrankie = frankie->height/2 + squirrel->childHeight/2;

    // Loop through all the missiles, then remove any squirrel that are within
	// "collisionDist" of the missiles.  the removeNear() function returns the
	// number of missiles removed.
	//
    for (int i = 0; i < frankie->sys->sprites.size(); i++) {


        /*if ((frankie->trans.y - squirrel->sys->sprites[i].trans.y) < collisionFrankie) {
            lives --;
        }*/
		score += squirrel->sys->removeNear(frankie->sys->sprites[i].trans, collisionDist);
        score += cat->sys->removeNear(frankie->sys->sprites[i].trans, collisionDist2);
    }
    
    
}



//  support dragging of frankie with mouse
//
void ofApp::mouseDragged(int x, int y, int button){
	ofPoint mouse_cur = ofPoint(x, y);
	ofVec3f delta = mouse_cur - mouseLast;

	// Allow frankie to be moved in lower half of screen only
	//
	if ((frankie->trans.y + delta.y) > ofGetWindowHeight() *.3 ) {
		frankie->trans += delta;
		mouseLast = mouse_cur;
	}
}

//--------------------------------------------------------------
// Start game if not running; otherwise track mouse last coord
// 
void ofApp::mousePressed(int x, int y, int button){
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


