#pragma once

#include "ofMain.h"

class ofApp : public ofBaseApp {
public:
	void setup();
	void update();
	void draw();
	
	void keyPressed(int key);
    
    ofMesh buildTube(const ofPolyline& path, int sides, float radius);
		
	int lastX, lastY;
	ofMesh mesh;
    ofPolyline line;
    
    ofEasyCam easycam;
};