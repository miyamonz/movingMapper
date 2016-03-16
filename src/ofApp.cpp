
#include "ofApp.h"
#include "InnoPocketDmx.h"
#define PORTR 3210
#define PORTS 51001

InnoPocketDmx* dmx[MOVING_NUM];
//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate(30);
    //add child
    float width = 200;
    movingManager = new MovingManager("manager");
    for(int i=0; i<MOVING_NUM;i++){
        RectMapper* rm = new RectMapper("moving");
        for(int j=0; j<ANCOUR_NUM; j++) {
            MapperPoint* m  = new MapperPoint("pt");
            m->add(new DegValue("pan",0,540));
            m->add(new DegValue("tilt",0,180));
            rm->add(m);
        }
        movingManager->add(rm);
    }
    movingManager->load("mapper.xml");
    
    movingManager->setupGui();
    receiver.setup(PORTR);
    sender.setup("localhost", PORTS);
//    sender.setup("localhost", 7701);
    
    for(int i=0; i<MOVING_NUM; i++){
        dmx[i] = new InnoPocketDmx(sender, 101+11*i);
    }
}

//--------------------------------------------------------------
void ofApp::update(){
    
	while(receiver.hasWaitingMessages()){
		// get the next message
		ofxOscMessage m;
		receiver.getNextMessage(m);
        
        for(int i=0; i<MOVING_NUM; i++){
    		if(m.getAddress() == "/movingManager/" + ofToString(i) + "/point"){
                receivedPoint[i].x = m.getArgAsFloat(0);
                receivedPoint[i].y = m.getArgAsFloat(1);
    		}
            if(m.getAddress() == "/movingManager/" + ofToString(i) + "/color"){
                color[i] = m.getArgAsString(0);
                dmx[i]->setColor(color[i]);
            }
            if(m.getAddress() == "/movingManager/" + ofToString(i) + "/gobo"){
                gobo[i] = m.getArgAsString(0);
                dmx[i]->setGobo(gobo[i]);
            }
            if(m.getAddress() == "/movingManager/" + ofToString(i) + "/strobe"){
                strobe[i] = m.getArgAsFloat(0);
                dmx[i]->setStrobe(strobe[i]);
            }
            if(m.getAddress() == "/movingManager/" + ofToString(i) + "/dimmer"){
                dimmer[i] = m.getArgAsFloat(0);
                dmx[i]->setDimmer(dimmer[i]);
            }


        }
	}

}
//--------------------------------------------------------------
void ofApp::draw(){
    //cam.begin();
    ofSetColor(100);
    ofDrawRectangle(0, 0, 640, 480);
    ofSetColor(ofColor::red);
    
    for(int i=0; i<MOVING_NUM; i++){
        if(i==0) ofSetColor(ofColor::red);
        if(i==1) ofSetColor(ofColor::green);
        if(i==2) ofSetColor(ofColor::blue);
        if(i==3) ofSetColor(ofColor::gray);
        float pan,tilt;
        int index = i;
        ofxXmlComposite* ptr = movingManager->getByTag("moving",index);
        ofxXmlCompositePack* pack =dynamic_cast<ofxXmlCompositePack* >(ptr);
        RectMapper* rect = dynamic_cast<RectMapper* >(pack);
        rect->update();
        rect->warper.draw();
        ofPoint calc = rect->getCalculatedPoint(receivedPoint[i]);
        
        
        
        ofDrawCircle(calc, 10);
        rect->toPanTilt(calc, pan, tilt);
        
        if(honbanMode){
            dmx[i]->setPan(pan);
            dmx[i]->setTilt(tilt);
            dmx[i]->send();
        }else{
            MapperPoint* pt =dynamic_cast<MapperPoint* >(rect->elements[kadoNum]);
            dmx[i]->setPan(pt->pan);
            dmx[i]->setTilt(pt->tilt);
            dmx[i]->send();
        }

        ofDrawBitmapStringHighlight(ofToString(dmx[i]->getPan()) + " " + ofToString(dmx[i]->getTilt()),200*i, 500);
    }
    //cam.end();
    ofDrawBitmapStringHighlight("receive " + ofToString(PORTR), 0,620);
    ofDrawBitmapStringHighlight("send    " + ofToString(PORTS), 0,640);
    ofDrawBitmapStringHighlight("mode    " + ofToString(honbanMode), 0,660);
    ofDrawBitmapStringHighlight("kado    " + ofToString(kadoNum),    0,680);
}


//--------------------------------------------------------------

void ofApp::keyPressed(int key){
    if(key == 's')
        movingManager->save("mapper.xml");
    if(key == 'l')
        movingManager->load("mapper.xml");
    if(key == ' ')
        honbanMode = !honbanMode;
    
    if(key == OF_KEY_UP) {
        if(kadoNum == 0) return;
        kadoNum--;
    }
    if(key == OF_KEY_DOWN) {
        if(kadoNum == 3) return;
        kadoNum++;
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
