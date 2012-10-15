#include "ofApp.h"

void ofApp::setup() {
	ofSetVerticalSync(true);
    openni.setup();
	image.setup(&openni);
	depth.setup(&openni);
	hands.setup(&openni, 4);
    hands.setMaxNumHands(1);    //track only one hand
	openni.registerViewport();
	openni.setMirror(true);
}

void ofApp::update() {
    openni.update();
	image.update();
	depth.update();
}

void ofApp::draw() 
{
    easycam.begin();    
    depth.draw(0, 0, 640, 480);
	ofBackground(0);
	ofSetColor(255);
    
	if(ofGetKeyPressed('c')) {
        line.clear();
	} 
    
    else {
        int n = hands.getNumTrackedHands();
        for(int i = 0; i < n; i++) {
            ofxTrackedHand* hand = hands.getHand(i);
            ofPoint& handPosition = hand->projectPos;
            line.addVertex(ofVec2f(handPosition.x-ofGetWidth()/2, handPosition.y-ofGetHeight()/2));
            line.simplify(0.3); //smooths line - removes extraneous pts
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

//mesh indices
int getIndex(int i, int j, int sides)
{
    return i * sides + (j % sides);
}

ofMesh ofApp::buildTube(const ofPolyline& path, int sides, float radius)   
{
    ofMesh mesh;
    mesh.setMode(OF_PRIMITIVE_TRIANGLES);
    
    //up vector orthogonal to polygonal slice surface
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
        
        //get perpendicular vectors with cross products
        ofVec3f u = diff.getCrossed(up).normalize() * radius;
        ofVec3f v = u.getCrossed(diff).normalize() * radius;
        
        //rotate around polyline building mesh
        for (int j = 0; j < sides; j++) {
            float theta = ofMap(j, 0, sides, 0, TWO_PI);
            float x = cos(theta);
            float y = sin(theta);
            ofVec3f pos = x*u + y*v + cur;
            mesh.addVertex(pos);
        }        
    
    }
    
    //closing tube sides - connect side indices to mesh by forming triangles
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
    
    //closing tube ends
    if (path.size() > 0){
        int centerStart = mesh.getNumVertices(); //center of tube start
        mesh.addVertex(path[0]);
        int centerEnd = mesh.getNumVertices();  //center of tube end
        mesh.addVertex(path[path.size()-1]);
        
        //connect side indices to centers of tube ends
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
