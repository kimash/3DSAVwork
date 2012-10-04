#include "ofApp.h"

void ofApp::setup() {
	ofSetVerticalSync(true);
	ofHideCursor();
}

void ofApp::update() {
}

void ofApp::draw() {
    //easycam.begin();
    
	ofBackground(0);
	ofSetColor(255);
	ofTranslate(ofGetWidth() / 2, ofGetHeight() / 2);
	ofTranslate(-ofGetWidth() / 2, -ofGetHeight() / 2);
    
    line.draw();
    
	if(ofGetKeyPressed('c')) {
        line.clear();
	} else {
        line.draw();
	}
    
    //easycam.end();
}

void ofApp::keyPressed(int key) {
	if(key == 'f') {
		ofToggleFullscreen();
	}
}

void ofApp::mouseDragged(int x, int y, int button) {
	ofVec2f start(lastX, lastY);
	ofVec2f end(x, y);
	ofVec2f diff = end - start;
    ofSetColor(255, 0, 0);
    line.addVertex(end);
	lastX = x;
	lastY = y;
    
}

ofMesh ofApp::buildTube(ofPolyline& path)   //add shapeOrder later
{
    ofMesh mesh;
    mesh.setMode(OF_PRIMITIVE_TRIANGLES);
    
    path.getVertices();
    
    
    mesh.save("out.ply");
    return mesh;
}
