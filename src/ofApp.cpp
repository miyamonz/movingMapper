
#include "ofApp.h"
#include "InnoPocketDmx.h"
#define PORTR 3210
#define PORTS 51001

InnoPocketDmx* dmx[MOVING_NUM*2];
//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate(30);
    //add child
    float width = 200;
    for(int k=0; k<2; k++) {
        movingManager[k] = new MovingManager("manager");
        for(int i=0; i<MOVING_NUM;i++){
            RectMapper* rm = new RectMapper("moving");
            rm->setGuiPosition(0, k*300);
            for(int j=0; j<ANCOUR_NUM; j++) {
                MapperPoint* m  = new MapperPoint("pt");
                m->add(new DegValue("pan",0,540));
                m->add(new DegValue("tilt",0,180));
                rm->add(m);
            }
            movingManager[k]->add(rm);
        }
    }
    for(auto i:{0,1}){
        movingManager[i]->load("mapper" + ofToString(i) + ".xml");
        movingManager[i]->setupGui();
    }
    
    receiver.setup(PORTR);
    sender.setup("localhost", PORTS);
    //    sender.setup("localhost", 7701);
    
    for(int k=0; k<2; k++)
        for(int i=0; i<MOVING_NUM; i++){
            dmx[MOVING_NUM*k + i] = new InnoPocketDmx(sender, 101+11*i);
        }
}



//--------------------------------------------------------------
void ofApp::update(){
    
    while(receiver.hasWaitingMessages()){
        // get the next message
        ofxOscMessage m;
        receiver.getNextMessage(m);
        for(int k=0; k<2; k++){
        for(int i=0; i<MOVING_NUM; i++){
            if(m.getAddress() == "/toMapper/" + ofToString(i) + "/point"){
                receivedPoint[i].x = m.getArgAsFloat(0);
                receivedPoint[i].y = m.getArgAsFloat(1);
            }
            if(m.getAddress() == "/toMapper/" + ofToString(i) + "/color"){
                color[i] = m.getArgAsString(0);
                dmx[MOVING_NUM*k + i]->setColor(color[i]);
                dmx[MOVING_NUM*k + i]->setColor(color[i]);
            }
            if(m.getAddress() == "/toMapper/" + ofToString(i) + "/gobo"){
                gobo[i] = m.getArgAsString(0);
                dmx[MOVING_NUM*k + i]->setGobo(gobo[i]);
            }
            if(m.getAddress() == "/toMapper/" + ofToString(i) + "/strobe"){
                strobe[i] = m.getArgAsFloat(0);
                dmx[MOVING_NUM*k + i]->setStrobe(strobe[i]);
            }
            if(m.getAddress() == "/toMapper/" + ofToString(i) + "/dimmer"){
                dimmer[i] = m.getArgAsFloat(0);
                dmx[MOVING_NUM*k + i]->setDimmer(dimmer[i]);
            }
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
    for(int k=0; k<2; k++ ){
        
        for(int i=0; i<MOVING_NUM; i++){
            if(i==0) ofSetColor(ofColor::red);
            if(i==1) ofSetColor(ofColor::green);
            if(i==2) ofSetColor(ofColor::blue);
            if(i==3) ofSetColor(ofColor::gray);
            float pan,tilt;
            int index = i;
            ofxXmlComposite* ptr = movingManager[k]->getByTag("moving",index);
            ofxXmlCompositePack* pack =dynamic_cast<ofxXmlCompositePack* >(ptr);
            RectMapper* rect = dynamic_cast<RectMapper* >(pack);
            rect->update();
            rect->warper.draw();
            ofPoint calc = rect->getCalculatedPoint(receivedPoint[i]);
            
            ofDrawCircle(calc, 10);
            rect->toPanTilt(calc, pan, tilt);
            
            if(honbanMode){
                dmx[MOVING_NUM*k + i]->setPan(pan);
                dmx[MOVING_NUM*k + i]->setTilt(tilt);
                dmx[MOVING_NUM*k + i]->send();
            }else{
                if(0+k*4 <= kadoNum && kadoNum < 4 + k*4){
                    MapperPoint* pt =dynamic_cast<MapperPoint* >(rect->elements[(kadoNum)%4]);
                    dmx[MOVING_NUM*k + i]->setPan(pt->pan);
                    dmx[MOVING_NUM*k + i]->setTilt(pt->tilt);
                    dmx[MOVING_NUM*k + i]->send();

                }

            }
            ofDrawBitmapStringHighlight(ofToString(dmx[MOVING_NUM*k + i]->getPan()) + " " + ofToString(dmx[MOVING_NUM*k + i]->getTilt()),50 + 200*i, 600 + k*20);
            
        }
        if(!honbanMode){
            if(kadoNum < 4){
                ofDrawCircle(20,600 -5,        10);
            }else{
                ofDrawCircle(20,600 -5 +20,    10);
            }
            
        }
    }
    //cam.end();
    int y = 20;
    ofDrawBitmapStringHighlight("receive " + ofToString(PORTR),      850,y += 20);
    ofDrawBitmapStringHighlight("send    " + ofToString(PORTS),      850,y += 20);
    ofDrawBitmapStringHighlight("mode    " + ofToString(honbanMode), 850,y += 20);
    ofDrawBitmapStringHighlight("kado    " + ofToString(kadoNum),    850,y += 20);
}


//--------------------------------------------------------------

void ofApp::keyPressed(int key){
    if(key == 's') {
        for(int k=0; k<2; k++) {
            movingManager[k]->save("mapper" + ofToString(k) + ".xml");
        }
    }
    if(key == 'l') {
        for(int k=0; k<2; k++) {
            movingManager[k]->load("mapper" + ofToString(k) + ".xml");
        }
    }
    if(key == ' ')
        honbanMode = !honbanMode;
    
    if(key == OF_KEY_UP) {
        if(kadoNum == 0) return;
        kadoNum--;
    }
    if(key == OF_KEY_DOWN) {
        if(kadoNum == 7) return;
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
