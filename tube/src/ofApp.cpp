#include "ofApp.h"

void ofApp::setup() {
	ofSetVerticalSync(true);
}

void ofApp::update() {
}

void ofApp::draw() {
    easycam.begin();
    
	ofBackground(0);
	ofSetColor(255);
    
	if(ofGetKeyPressed('c')) {
        line.clear();
	} else if(ofGetKeyPressed(' ')) {
        if(mouseX != ofGetPreviousMouseX() &&
           mouseY != ofGetPreviousMouseY()) {
            line.addVertex(ofVec2f(mouseX-ofGetWidth()/2, mouseY-ofGetHeight()/2));
        }
	}
    line.draw();
    
    mesh = buildTube(line, 6, 40);
    ofSetColor(ofColor::red);
    mesh.draw();
    ofSetColor(0);
    mesh.drawWireframe();
    
    easycam.end();
}

void ofApp::keyPressed(int key) {
	if(key == 'f') {
		ofToggleFullscreen();
	}
    if(key == 's') {
        mesh.save("out.ply");
    }
}

void ofApp::mouseDragged(int x, int y, int button) {

}

int getIndex(int i, int j, int sides)
{
    return i * sides + (j % sides);
}

ofMesh ofApp::buildTube(const ofPolyline& path, int sides, float radius)   //add shapeOrder later
{
    ofMesh mesh;
    //mesh.setMode(OF_PRIMITIVE_LINE_STRIP);
    mesh.setMode(OF_PRIMITIVE_TRIANGLES);
    
    ofVec3f up(0, 1, 0);
    for (int i = 0; i < path.size(); i++)
    {
        ofVec3f diff, cur, next;
        if(i + 1 == path.size()) {
            cur = path[i];
            next = path[i-1];
            diff = cur - next;
        } else {
            cur = path[i];
            next = path[i+1];
            diff = next - cur;
        }
        ofVec3f u = diff.getCrossed(up).normalize() * radius;
        ofVec3f v = u.getCrossed(diff).normalize() * radius;
        
        for (int j = 0; j < sides; j++) {
            float theta = ofMap(j, 0, sides, 0, TWO_PI);
            float x = cos(theta);
            float y = sin(theta);
            ofVec3f pos = x*u + y*v + cur;
            mesh.addVertex(pos);
        }        
    
    }
    
    for (int i = 0; i+1 < path.size(); i++)
    {
        for (int j = 0; j < sides; j++) {
            mesh.addIndex(getIndex(i, j, sides));
            mesh.addIndex(getIndex(i+1, j, sides));
            mesh.addIndex(getIndex(i, j+1, sides));
            mesh.addIndex(getIndex(i+1, j, sides));
            mesh.addIndex(getIndex(i+1, j+1, sides));
            mesh.addIndex(getIndex(i, j+1, sides));
        }
    }
    
    if (path.size() > 0){
        int centerStart = mesh.getNumVertices();
        mesh.addVertex(path[0]);
        int centerEnd = mesh.getNumVertices();
        mesh.addVertex(path[path.size()-1]);
        
        for (int j = 0; j < sides; j++) {
            mesh.addIndex(centerStart);
            mesh.addIndex(getIndex(0, j, sides));
            mesh.addIndex(getIndex(0, j+1, sides));
            
            mesh.addIndex(centerEnd);
            mesh.addIndex(getIndex(path.size()-1, j+1, sides));
            mesh.addIndex(getIndex(path.size()-1, j, sides));
        }

    }
    return mesh;
}
