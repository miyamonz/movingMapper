#pragma once
#include "ofMain.h"
#include "ofxOsc.h"

class InnoPocketDmx {
    ofxOscSender sender;
    int address;
    
    float pan  =0;
    float tilt =0;
    
public:
    InnoPocketDmx(){};

    InnoPocketDmx(ofxOscSender &_sender, int _address) : sender(_sender), address(_address) {
    }
    void setSender(ofxOscSender &_sender){
        sender = _sender;
    }
    void setAddress(int _address){
        address = _address;
    }
    void setPan(float _pan){
        pan = ofMap(_pan, 0, 540,0, 255);
    }
    float getPan(){
        return pan;
    }
    void setTilt(float _tilt){
        tilt = ofMap(_tilt, 0, 180,0,255);
    }
    float getTilt(){
        return tilt;
    }
    
    void send(){
        sendPan();
        sendTilt();
        ofLog() << "send " << ofToString(address);
    }
    
    void sendPan(){
        ofxOscMessage m;
        m.setAddress("/dmx/" + ofToString(address));
        m.addIntArg((int)pan);
        sender.sendMessage(m);
        m.clear();
        m.setAddress("/dmx/" + ofToString(address+1));
        m.addIntArg((pan - (int)pan)*255);
        sender.sendMessage(m);
    }
    
    void sendTilt(){
        ofxOscMessage m;
        m.setAddress("/dmx/" + ofToString(address+2));
        m.addIntArg((int)tilt);
        sender.sendMessage(m);
        m.clear();
        m.setAddress("/dmx/" + ofToString(address+3));
        m.addIntArg((tilt - (int)tilt)*255);
        sender.sendMessage(m);
    }
};