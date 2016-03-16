    #include "ofMain.h"
#include "ofxDatGui.h"
#include "ofxQuadWarp.h"
#include "ofxXmlSettings.h"
#include "ofxXmlComposite.h"


class DegValue : public ofxXmlCompositeLeaf {
public:
    float val;
    ofxDatGuiSlider* sl;
    float min , max;
    DegValue(string tagName,float min,float max) : ofxXmlCompositeLeaf(tagName) {
        this->min = min;
        this->max = max;
    }
    float getValue(){
        return val;
    }
    
    void setupGui(ofxDatGui* gui,int i){
        sl = gui->addSlider(tagName+ofToString(i),min,max);
        sl->bind(this->val);
        //sl->onSliderEvent();
    }
    
    void save(ofxXmlSettings &xml){
        xml.addTag(tagName);
        xml.setValue(tagName ,val , 0);
    }
    void load(ofxXmlSettings &xml,int i){
        float xmlval = xml.getValue(tagName,0.);
        val = xmlval;
    }
    void sliderEvent(){
        
    }

};

class MapperPoint :public ofxXmlCompositePack {
public:
    float pan=0;
    float tilt=90;
    float z = -100;
    float length;
    ofPoint p;
    MapperPoint(string tagName) : ofxXmlCompositePack(tagName) {}
    ofPoint getPoint(){
        pan  = dynamic_cast<DegValue* >(dynamic_cast<ofxXmlCompositeLeaf* >(getByTag("pan")))->val;
        tilt = dynamic_cast<DegValue* >(dynamic_cast<ofxXmlCompositeLeaf* >(getByTag("tilt")))->val;
        //tilt = getBeTag("tilt",0);
        p.z = z;
        length = p.z / sin(ofDegToRad(tilt));
        float r = length*cos(ofDegToRad(tilt));
        p.x = r*cos(ofDegToRad(pan));
        p.y = r*sin(ofDegToRad(pan));
        return p;
    };
    void setupGui(ofxDatGui* gui, int i){
        ofxXmlCompositePack::setupGui(gui,i);
        gui->addBreak()->setHeight(10);
    }
    
};

class RectMapper : public ofxXmlCompositePack {
public:
    ofxQuadWarp warper;
    float z=-100;
    RectMapper(string s) : ofxXmlCompositePack(s){
        
        int w = 640;
        int h = 480;
        warper.setSourceRect(ofRectangle(0,0,w,h));
        warper.setCorner(ofPoint(0,0),0);
        warper.setCorner(ofPoint(w,0),1);
        warper.setCorner(ofPoint(w,h),2);
        warper.setCorner(ofPoint(0,h),3);
        warper.setup();
    }
    
    ofxDatGui* gui;
    int width = 200;
    void setupGui(int i){
        this->gui = new ofxDatGui();
        this->gui->addHeader(tagName+ofToString(i));
        this->gui->setPosition(i*(width+10),0);
        this->gui->setWidth(width);
        for(int j=0; j<elements.size();j++){
            elements[j]->setupGui(this->gui,j);
        }
    }
    void update(){
        for(int i=0; i<elements.size(); i++){
            ofPoint p = dynamic_cast<MapperPoint* >(elements[i])->getPoint();
            p.z = 0;
            warper.setCorner(p,i);
        }
    }
    ofVec4f getCalculatedPoint(ofPoint p){
        p.z = 0;
        ofPoint matrixed = p*warper.getMatrix();
        ofVec4f r = ofVec4f(matrixed.x, matrixed.y, 0, 1);
        r = r / r.w;
        return r;
    }
    void toPanTilt(ofPoint p, float &pan, float &tilt){
        float length = p.length();
        pan = ofVec2f(1,0).angle(p);//degree
        tilt = 90 - ofRadToDeg(atan(length/z));//zをもっとよくすべき
    };
};

class MovingManager : public ofxXmlCompositePack {
public:
    string fileName;
    int width = 200;
    ofxXmlSettings xml;
    
    MovingManager(string tagName) :ofxXmlCompositePack(tagName) {
    }
    void save(string fileName){
        xml.clear();
        ofxXmlCompositePack::save(xml);
        xml.saveFile(fileName);
    }
    void load(string fileName){
        this->fileName = fileName;
        xml.loadFile(fileName);
        ofxXmlCompositePack::load(xml,0);
    }
    void setupGui(){
        ofxXmlCompositePack::setupGui(0);
    }
    ofxXmlComposite* getByTag(string tagname, int index){
        return ofxXmlCompositePack::getByTag(tagname, index);
    }
};