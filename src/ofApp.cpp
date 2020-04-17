#include "ofApp.h"
#include "Emitter.h"

//GAME PLAY
//Press space to start
//Move frankie using mouse and fire using space bar. If frankie is moved using the mouse,
//direction of emitter is right. If frankie is moved
//using arrow keys or not being moved at all, emitter direction is determined by mouse click.
//frankie will not move unless mouse is in the area of the frankie
//Sound will play during each rate of fire from the frankie
//Each enemy is worth different points. You start off with 5 lives, each life goes down by one
//if you collide with any enemies. You can gain lives by picking up bones on the ground
//Fire hydrants emit water and mailman drops mail. If you hit the mailman, it explodes with
//mail and is worth 10 points. Each level is progressed ever 30 seconds, try to get as many
//point as possible before losing all your lives!
//
//Julie Jacobson - April 15, 2020
//
//BUGS: background begins to glitch after awhile
//TODO: (things I want to complete past pt 3) Get cat and squirrel sprites to animate
//TODO: get squirrel to move at different heights
//
//--------------------------------------------------------------
void ofApp::setup(){
    ofSetVerticalSync(true);
    score = 0;
    
    
    
    // Record when the game starts - game will run for 10 sec
    //
    gameStartTime = ofGetElapsedTimeMillis();
    
    // Create and setup emitters
    frankie = new Emitter( new SpriteSystem());
    squirrel = new Emitter(new SpriteSystem());
    cat = new Emitter(new SpriteSystem());
    squirrel->s = new Sprite();
    mailman = new Emitter(new SpriteSystem());
    firehydrant = new Emitter(new SpriteSystem());
    bone = new Emitter(new SpriteSystem()); 

    //initialize background position
    grassPos = glm::vec3(0,0,0);
    grassPos2 = glm::vec3(ofGetWindowWidth(), 0,0);
    cloudPos = glm::vec3(0,0,0);
    cloudPos2 = glm::vec3(ofGetWindowWidth(), 0,0);
    
    
    //Load files
    //
    ofImage frankieImg;
    if (!frankieImg.load("images/frankieAnim.png")) {
        ofLogFatalError("can't load image: images/frankieAnim.png");
        ofExit();
    } if (!background.load("images/background.png")){
        ofLogFatalError("can't load image: images/background.png");
        ofExit();
    } if (!grass.load("images/grass.png")) {
        ofLogFatalError("can't load image: images/grass.png");
        ofExit();
    } if(!cloud.load("images/clouds.png")) {
        ofLogFatalError("can't load image: images/clouds.png");
        ofExit();
    } if (!meow.load("images/meow.png")) {
        ofLogFatalError("can't load image: images/meow.png");
        ofExit();
    } if (!woof.load("images/woof.png")) {
        ofLogFatalError("can't load image: images/woof.png");
        ofExit();
    } if (!squirrelImg.load("images/squirrel.png")) {
        ofLogFatalError("can't load image: images/squirrel.png");
    } if (!catImg.load("images/cat.png")) {
        ofLogFatalError("can't load image: images/cat.png");
        ofExit();
    } if (!shoot.load("sounds/woofSound.mp3")) {
        ofLogFatalError("can't load sound: sounds/woofSound.mp3");
        ofExit();
    } if (!prologue.load("sounds/prologue.mp3")) {
        ofLogFatalError("can't load sound: sounds/prologue.mp3");
        ofExit();
    } if (!chibiNinja.load("sounds/chibiNinja.mp3")) {
        ofLogFatalError("can't load sound: sounds/chibiNinja.mp3");
        ofExit();
    } if(!mailmanImg.load("images/mailman.png")) {
        ofLogFatalError("can't load image: images/mailman.png");
        ofExit();
    } if(!fireImg.load("images/firehydrant.png")) {
        ofLogFatalError("can't load image: firehydrant.png");
        ofExit();
    } if(!mailImg.load("images/mailImg.png")) {
        ofLogFatalError("can't load image: mailImg.png");
        ofExit();
    } if(!waterImg.load("images/water.png")) {
        ofLogFatalError("can't load image: water.png");
        ofExit();
    } if(!boneImg.load("images/bone.png")) {
        ofLogFatalError("can't load image: bone.png");
        ofExit(); 
    } if(!pickup.load("sounds/pickup.wav")) {
        ofLogFatalError("can't load sound: pickup.wav");
        ofExit();
    } if(!lose.load("sounds/hitFrankie.wav")) {
        ofLogFatalError("can't load sound: hitFrankie.wav");
        ofExit();
    }
    
    //Set up setting for sounds
    //
    shoot.setMultiPlay(true);
    shoot.setVolume(.3);
    pickup.setMultiPlay(true);
    lose.setMultiPlay(true);
    chibiNinja.play();
    chibiNinja.setLoop(true);
    chibiNinja.setMultiPlay(true);
    prologue.setMultiPlay(true);
    
    
    //Set up main emitter
    //
    woof.resize(56,56);
    frankie->setImage(frankieImg);
    frankie->setAnimImage(true);
    frankie->setPosition(ofVec3f(10, ofGetWindowHeight()/2, 0));
    frankie->setVelocity(ofVec3f(1000, 0, 0));
    frankie->setRate(0);
    frankie->setLifespan(2000);
    frankie->setChildImage(woof);
    frankie->drawable = true;
    frankie->width = 128;
    frankie->height = 128;
    frankie->ntiles_x = 4;
    frankie->ntiles_y = 2;
    frankie->voff = 0;
    frankie->hoff = 0;
    frankie->nframes = 8;
    
    
    //Set up enemy emitters
    //
    squirrel->setPosition(ofVec3f(ofGetWindowWidth() - 10, ofGetWindowHeight()/2, 0));
    squirrel->velocity.set(-400, 0, 0);
    squirrel->setLifespan(5000);
    squirrel->setRate(0);
    squirrel->drawable = false;
    squirrel->setChildImage(squirrelImg);
    
    mailman->drawable=false;
    mailman->setPosition(ofVec3f(ofGetWindowWidth() - 20, ofGetWindowHeight() * .205, 0));
    mailman->setChildImage(mailmanImg);
    mailman->velocity.set(-500, 0, 0);
    mailman->setRate(0);
    mailman->setLifespan(10000);
    
    firehydrant->drawable = false;
    firehydrant->setPosition(ofVec3f(ofGetWindowWidth() - 10, ofGetWindowHeight()*.5, 0));
    firehydrant->setChildImage(fireImg);
    firehydrant->velocity.set(-300, 0, 0);
    firehydrant->setRate(0);
    firehydrant->setLifespan(10000);

    cat->setPosition(ofVec3f(ofGetWindowWidth() - 10, ofGetWindowHeight()/2, 0));
    cat->velocity.set(-300,0,0);
    cat->setLifespan(10000);
    cat->setRate(0);
    cat->setChildImage(catImg);
    cat->drawable = false;
    
    bone->setPosition(ofVec3f(ofGetWindowWidth() - 10, ofGetWindowHeight()/2, 0));
    bone->velocity.set(-100,0,0);
    bone->setLifespan(20000);
    bone->setRate(0);
    bone->setChildImage(boneImg);
    bone->drawable = false;
    
    //Set up particle emitters for enemies
    water.particleRadius = 3;
    water.particleColor = ofColor(255, 255, 255);
    water.setLifespan(3000);
    water.setRate(300);
    water.setRandomLife(true);
    water.setGroupSize(25);
    
    mail.particleColor = ofColor(255, 255, 255);
    mail.setRate(0);
    mail.setLifespan(3);
    mail.setEmitterType(RadialEmitter);
    mail.setVelocity(ofVec3f(50, 50));
    
    
    //Add forces to the particles
    //
    ParticleSystem *sys2 = mail.sys;
    ParticleSystem *sys = water.sys;
    sys->addForce(new GravityForce(ofVec3f(-200, 200, 0)));
    sys2->addForce(new TurbulenceForce(ofVec3f(-4, -5, 0), ofVec3f(10, 10, 0)));
    sys2->addForce(new GravityForce(ofVec3f(0, 10, 0)));
    
    
    //Start up all emitters
    //
    firehydrant->start();
    mailman->start();
    cat->start();
    squirrel->start();
    frankie->start();
    bone->start();
    mail.start();
    water.start();
}

//--------------------------------------------------------------
//  ofApp::update() is called every frame by the OF mainloop.
//  If the frame rate is 60 fps (frames per second) it gets called
//  every 1/60 sec
//
void ofApp::update() {
    

    //When spacebar is pressed, frankie becomes animated
    //Background scrolls and missles fire when space bar is held down
    //
    if (startGame) {

        //Set frankie to animate
        frankie->bAnimRunning = true;
        //Change position of background grass and clouds
        grassPos.x -= 250 * ofGetLastFrameTime();
        grassPos2.x -= (250 * ofGetLastFrameTime()) - ofGetWindowWidth();
        cloudPos.x -= 50 * ofGetLastFrameTime();
        cloudPos2.x -= (50 * ofGetLastFrameTime()) - ofGetWindowWidth();
        
        //Randomize firehydrant and mailman velocities
        firehydrant->velocity = ofVec3f(ofRandom(-100, -300), 0, 0);
        firehydrant->trans[1] = ofRandom(ofGetWindowHeight(), ofGetWindowHeight()*.3);
        mailman->velocity = ofVec3f(ofRandom(-500, -900), 0, 0);
        
        //If space bar is pressed, frankie barks and woof is emitted
        if (!bFire) frankie->setRate(0);
        else {
            frankie->setRate(4);
            shoot.play();
        }
        
        //Set up particle images for mail man
        for (int i = 0 ; i < mail.sys->particles.size(); i++) {
            mail.sys->particles[i].drawable = true;
            mail.sys->particles[i].setImage(mailImg);
        }
        
        //Level up by 30 seconds
        switch((int)ofGetElapsedTimef()) {
            case 30: level = 2;
                break;
            case 60: level = 3;
                break;
            case 90: level = 4;
                break;
        }

        
        //Set up emitters for each level
        if (level == 1) {
            bone->setRate(.1);
            cat->setRate(.75);
        } else if (level == 2) {
            squirrel->setRate(2);
        } else if (level == 3) {
            firehydrant->setRate(2);
        } else if (level == 4) {
            mailman->setRate(.2);
        }
        
    }
    
    
    //If frankie is not being moved with mouse, use mouse click to change direction
    //If the frankie IS being moved with the mouse, use default
    if (bDrag) {
        frankie->setVelocity(glm::vec3(1000, 0, 0));
    } else frankie->setVelocity(mouseLast - frankie->trans);

    //Find when frankie's emitter his mail man to initiate mail explosion
    for (int i = 0; i < frankie->sys->sprites.size(); i++) {
        for (int j = 0; j < mailman->sys->sprites.size(); j++) {
            ofVec3f v;
            v[0] = mailman->sys->sprites[j].trans[0] + 200;
            v[1] = mailman->sys->sprites[j].trans[1] + 100;
            mail.setPosition(v);
            mail.setRate(10);
        }
    }
    
    //Set up position of water emitter on firehydrant
    for (int i = 0; i < firehydrant->sys->sprites.size(); i ++) {
        ofVec3f v;
        v[0] = firehydrant->sys->sprites[i].trans[0] + 124;
        v[1] = firehydrant->sys->sprites[i].trans[1] + 124;
        water.setPosition(v);
    }
    int randomx = ofRandom(-300, -100);
    water.setVelocity(ofVec3f(randomx, -200, 0));
    
    //Check for collisions
    checkCollisions();
    
    //Squirrel is emitted in a sine wave using curveEval function
    squirrel->sys->amplitude = 100;
    squirrel->sys->cycles = 2;
    squirrel->sys->yposition = ofGetWindowHeight()*.7;
    squirrel->sys->bwave = true;
    for (int i =0; i < squirrel->sys->sprites.size() ; i++) {
        squirrel->sys->sprites[i].xposition -= 5;
    }
    
    //Cat is emitted at random height in game
    float random = ofRandom(.3 ,1);
    cat->trans.y = ofGetWindowHeight() * random;
    if (random > .6){
        cat->setVelocity(ofVec3f(cat->velocity.x, -50, cat->velocity.z));
    } else  cat->setVelocity(ofVec3f(cat->velocity.x,ofRandom(-cat->velocity.x /2, 0), cat->velocity.z));
    
    //Bone is emitted at random heights in the game
    bone->trans.y = ofGetWindowHeight() * random;
    
    //Call emitter update functions
    firehydrant->update();
    mailman->update();
    frankie->update();
    squirrel->update();
    cat->update();
    bone->update();
    water.update();
    mail.update();
    
    //If run out of lives, game is over
    if (frankie->lives == 0) gameOver = true;
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
    //TODO: (bug) grass background stops being seamless after awhile
    if (!startGame) {
        grass.draw(0,0,ofGetWindowWidth(), ofGetWindowHeight());
        cloud.draw(0,0,ofGetWindowWidth(), ofGetWindowHeight());
        ofDrawBitmapString("PRESS SPACE TO START", ofPoint(ofGetWindowWidth() / 2, ofGetWindowHeight() / 2));
    } else {
        mailman->draw();
        grass.draw( int(grassPos.x) % ofGetWindowWidth(), grassPos.y, ofGetWindowWidth(), ofGetWindowHeight());
        grass.draw(int(grassPos2.x) % ofGetWindowWidth(), grassPos2.y, ofGetWindowWidth(), ofGetWindowHeight());
        
        cloud.draw(int(cloudPos.x) % ofGetWindowWidth(), cloudPos.y, ofGetWindowWidth(), ofGetWindowHeight());
        cloud.draw(int(cloudPos2.x) % ofGetWindowWidth(), cloudPos2.y, ofGetWindowWidth(), ofGetWindowHeight());
    }
    
    
    //Once game is over, draw "GAME OVER" text on screen
    if (gameOver) {
        chibiNinja.stop();
        prologue.play();
        
        string gameOver = "GAME OVER\nSCORE: " + std::to_string(score) + "\nLEVEL: " + std::to_string(level);
        ofDrawBitmapString(gameOver, ofPoint(ofGetWindowWidth() / 2, ofGetWindowHeight() / 2));
        
        mailman->stop();
        frankie->stop();
        squirrel->stop();
        cat->stop();
        firehydrant->stop();
        bone->draw();
    }
    else {
        firehydrant->draw();
        frankie->draw();
        cat->draw();
        mailman->draw();
        squirrel->draw();
        water.draw();
        mail.draw();
        bone->draw();
        
    }

    //Draw score
    string scoreText;
    string lifeText;
    string levelText;
    scoreText += "Score: " + std::to_string(score);
    ofDrawBitmapString(scoreText, ofPoint(10, 20));
    lifeText += "Lives Remaining: " + std::to_string(frankie->lives);
    ofDrawBitmapString(lifeText, ofPoint(10, 50));
    levelText += "Level: " + std::to_string(level);
    ofDrawBitmapString(levelText, ofPoint(10, 80));
}

//  This is a simple O(M x N) collision check
//  For each missle check to see which squirrel you hit and remove them
//
void ofApp::checkCollisions() {
//hit.play();

    // find the distance at which the two sprites (missles and squirrel) will collide
    // detect a collision when we are within that distance.
    //
    float squirrelcollision = frankie->childWidth / 2 + squirrel->childWidth / 2;
    float catcollision = frankie->childWidth / 2 + cat->childWidth / 2;
    float mailcollision = frankie->childWidth / 2 + mailman->childWidth / 2;
    float firecollision = frankie->childWidth / 2 + firehydrant->childWidth / 2;
    float bonecollision = frankie->childWidth / 2 + bone->childWidth /2;
    
    //Set up points for each emitter
    squirrel->sys->points = 1;
    cat->sys->points = 2;
    firehydrant->sys->points = 3;
    mailman->sys->points = 10;
    
    // Loop through all the missiles, then remove any squirrel that are within
    // "collisionDist" of the missiles.  the removeNear() function returns the
    // number of missiles removed.
    //
    for (int i = 0; i < frankie->sys->sprites.size(); i++) {
        score += squirrel->sys->removeNear(frankie->sys->sprites[i].trans, squirrelcollision);
        score += cat->sys->removeNear(frankie->sys->sprites[i].trans - frankie->sys->sprites[i].height, catcollision);
        score += mailman->sys->removeNear(frankie->sys->sprites[i].trans - frankie->sys->sprites[i].height, mailcollision);
        score += firehydrant->sys->removeNear(frankie->sys->sprites[i].trans - frankie->sys->sprites[i].height, firecollision);
        
    }

    //Call lose a life for each emitter
    loseALife(squirrel, squirrelcollision);
    loseALife(cat, catcollision);
    loseALife(mailman, mailcollision);
    loseALife(firehydrant, firecollision);
    
    //Call gain a life for bones
    gainALife(bone, bonecollision);
}

//Checks to see if frankie collides with enemies, loses a life if she does
void ofApp::loseALife(Emitter *e, float dist) {
    
    for (int i = 0; i < e->sys->sprites.size(); i++) {
        ofVec3f v = e->sys->sprites[i].trans - frankie->trans;
        if (v.length() < dist) {
            e->sys->remove(i);
            frankie->lives --;
            lose.play();
        }
    }
}

//Checks to see if frankie collides with bone, gains a life if she does
void ofApp::gainALife(Emitter *e, float dist) {
    
    for (int i = 0; i < e->sys->sprites.size(); i++) {
        ofVec3f v = e->sys->sprites[i].trans - frankie->trans;
        if (v.length() < dist) {
            e->sys->remove(i);
            frankie->lives ++;
            pickup.play();
        }
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

