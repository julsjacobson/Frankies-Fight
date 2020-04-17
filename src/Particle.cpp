#include "Particle.h"


Particle::Particle() {

	// initialize particle with some reasonable values first;
	//
	velocity.set(0, 0, 0);
	acceleration.set(0, 0, 0);
	position.set(0, 0, 0);
	forces.set(0, 0, 0);
    angle = 0; 
	lifespan = 5;
	birthtime = 0;
	radius = .1;
	damping = .99;
	mass = 1;
	color = ofColor::aquamarine;
}

void Particle::setImage (ofImage i) {
    image = i;
}

void Particle::draw() {
	
//	ofSetColor(ofMap(age(), 0, lifespan, 255, 10), 0, 0);
	
    
    if(drawable) {
        image.draw(position); 
    } else {
        ofDrawSphere(position, radius);
        ofSetColor(color);
    }
        
}

void Particle::integrate() {
	// check for 0 framerate to avoid divide errors
	//
	float framerate = ofGetFrameRate();

	// avoid accumulating forces on startup before frame is drawn
	//
	if (framerate < 5.0)
	{
		forces.set(0, 0, 0);
		return;
	}

	// interval for this step
	//
	float dt = 1.0 / framerate;

	// update position based on velocity
	//
	position += (velocity * dt);
    angle += velocity.x * dt;

	// update acceleration with accumulated paritcles forces
	// remember :  (f = ma) OR (a = 1/m * f)
	//
	ofVec3f accel = acceleration;    // start with any acceleration already on the particle
	accel += (forces * (1.0 / mass));
	velocity += accel * dt;


	// add a little damping for good measure
	//
	velocity *= damping;

	// clear forces on particle (they get re-added each step)
	//
	forces.set(0, 0, 0);
}

//  return age in seconds
//
float Particle::age() {
	return (ofGetElapsedTimeMillis() - birthtime)/1000.0;
}


