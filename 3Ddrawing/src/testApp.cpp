#include "testApp.h"

const float FovX = 1.0144686707507438;
const float FovY = 0.78980943449644714;
const float XtoZ = tanf(FovX / 2) * 2;
const float YtoZ = tanf(FovY / 2) * 2;
const unsigned int Xres = 640;
const unsigned int Yres = 480;

ofVec3f ConvertProjectiveToRealWorld(float x, float y, float z) {
	return ofVec3f((x / Xres - .5f) * z * XtoZ,
								 (y / Yres - .5f) * z * YtoZ,
								 z);
}

void testApp::setup() 
{
	ofSetVerticalSync(true);
	kinect.init(false, false);  // disable infrared/rgb video iamge (faster fps)
	kinect.open();
	useProjective = true;
    mesh.setMode(OF_PRIMITIVE_LINE_STRIP);
}

void testApp::update() {
	kinect.update();
}

void testApp::draw() 
{
	ofBackground(0);
	
	ofSetColor(255);
	kinect.drawDepth(0, 0, 320, 240);
	
	easyCam.begin();
	int width = kinect.getWidth();
	int height = kinect.getHeight();
	ofScale(1, -1, -1); // orient the point cloud properly
    
	if(useProjective) {
		ofTranslate(-width / 2, -height / 2); // center the projective point cloud
	} 
    
    else {
		ofTranslate(0, 0, -1500); // rotate about z = 1500 mm
	}
    
	float* distancePixels = kinect.getDistancePixels(); // distance in millimeters
    
	ofMesh cloud;
	cloud.setMode(OF_PRIMITIVE_POINTS);
    
//    float minZ = 20000;
//    float foreX = 0;
//    float foreY = 0;
    forePt.z = 20000;
    
	for(int y = 0; y < height; y++) {
		for(int x = 0; x < width; x++) {
			int i = y * width + x;
			float z = distancePixels[i];
            
//            if (z < minZ && z != 0) {
//                minZ = z;
//                foreX = x;
//                foreY = y;
//                mesh.addVertex(ofVec3f(foreX, foreY, minZ));

			if(z != 0) { // ignore empty depth pixels
				if(useProjective) {
					cloud.addVertex(ofVec3f(x, y, z));
                    
                    if(z < forePt.z){
                        forePt = ofVec3f(x, y, z);
                    }

				} else {
					cloud.addVertex(ConvertProjectiveToRealWorld(x, y, z));
                    
                    if(z < forePt.z){
                        forePt = ConvertProjectiveToRealWorld(x,y,z);
                    }
				}
			}
		}
	}
    
    mesh.addVertex(ofVec3f(forePt.x, forePt.y, forePt.z));
    
	cloud.drawVertices();
    
//    ofTranslate(foreX, foreY, minZ);
//    ofTranslate(forePt.x, forePt.y, forePt.z);
    ofSetColor(255, 0, 0);
    ofSphere(forePt.x, forePt.y, forePt.z, 10);

    
    ofSetColor(0, 255, 255);
    ofSetLineWidth(5);
    mesh.draw();

//	easyCam.end();
    
	
	ofSetColor(255);
	ofDrawBitmapString(useProjective ? "projective point cloud" : "real world point cloud", 10, 20);
    
    easyCam.end();
}

void testApp::exit() {
	kinect.close();
}

void testApp::keyPressed(int key) {
	if(key == ' ') {
		useProjective = !useProjective;
	}
}

