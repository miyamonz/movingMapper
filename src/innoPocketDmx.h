#pragma once
#include "ofMain.h"
#include "ofxOsc.h"
#include "innoPocket.h"

class InnoPocketDmx {
    ofxOscSender sender;
    int address;
    
    float pan  =0;
    float tilt =0;
    
    int  colorDmx   = 0;
    int  goboDmx    = 0;
    int   strobeDmx = 0;
    int   dimmerDmx = 0;
    
    
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
    void setColor(string _color){
        colorDmx = inno::getDmxFromColorString(_color);
    }
    void setGobo(string _gobo){
        goboDmx = inno::getDmxFromGobo(_gobo);
    }
    void setStrobe(float _strobe){
        strobeDmx = inno::getDmxFromStrobe(_strobe);
    }
    void setDimmer(float _dimmer){
        dimmerDmx = inno::getDmxFromDimmer(_dimmer);
    }
    void send(){
        sendPan();
        sendTilt();
        sendColor();
        sendGobo();
        sendStrobe();
        sendDimmer();
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
    void sendColor(){
        ofxOscMessage m;
        m.setAddress("/dmx/" + ofToString(address+4));
        m.addIntArg(colorDmx);
        sender.sendMessage(m);
    }
    void sendGobo(){
        ofxOscMessage m;
        m.setAddress("/dmx/" + ofToString(address+5));
        m.addIntArg(goboDmx);
        sender.sendMessage(m);
    }
    void sendStrobe(){
        ofxOscMessage m;
        m.setAddress("/dmx/" + ofToString(address+6));
        m.addIntArg(strobeDmx);
        sender.sendMessage(m);
    }
    void sendDimmer(){
        ofxOscMessage m;
        m.setAddress("/dmx/" + ofToString(address+7));
        m.addIntArg(dimmerDmx);
        sender.sendMessage(m);
    }
};