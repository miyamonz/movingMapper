#pragma once

#include "ofMain.h"
#include "ofxDatGui.h"
#include "ofxXmlSettings.h"
#include "ofxOsc.h"

//#include "movingManager.h"
#include "movingManagerWithComposite.h"
#define MOVING_NUM 4
#define ANCOUR_NUM 4

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		
    ofPoint p;
    MovingManager* movingManager;
    ofxDatGui* gui;
    ofEasyCam cam;
    
    ofxOscReceiver receiver;
    ofxOscSender sender;
    ofPoint receivedPoint[MOVING_NUM];
    
    bool honbanMode = false;
    float pan[MOVING_NUM];
    float tilt[MOVING_NUM];
    
    int kadoNum = 0;
    
    
};
