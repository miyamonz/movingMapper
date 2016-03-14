//#include "ofMain.h"
//#include "ofxDatGui.h"
//#include "ofxQuadWarp.h"
//#include "ofxXmlSettings.h"
//#include "ofxXmlComposite.h"
//
//#define MOVING_NUM 4
//#define ANCOUR_NUM 4
//
//
//
//class Mapper {
//public:
//    float pan=0;
//    float tilt=90;
//    float z;
//    float length;
//    ofPoint p;
//    ofxDatGui* gui;
//    ofxXmlSettings xml;
//    Mapper() {
//    }
//    Mapper(float z) {
//        this->z = z;
//        gui = new ofxDatGui(0,0);
//        gui->setOpacity(0.5);
//        gui->addHeader("pan tilt");
//        gui->addSlider("pan", 0,360)->bind(pan);
//        gui->addSlider("tilt",0,180)->bind(tilt);
//        gui->onSliderEvent(this, &Mapper::onSlider);
//        //gui->setVisible(false);
//    }
//    void addTag(ofxXmlSettings &xml){
//        xml.addTag("pan");
//        xml.addTag("tilt");
//    }
//    void save(ofxXmlSettings &xml){
//        xml.setValue("pan" , pan, 0);
//        xml.setValue("tilt", tilt,0);
//    }
//    void load(ofxXmlSettings &xml){
//        pan  = xml.getValue("pan",  0.);
//        tilt = xml.getValue("tilt", 0.);
//    }
//    void onSlider(ofxDatGuiSliderEvent e){
//        toPoint(p);
//    }
//    void toPoint(ofPoint &p){
//        p.z = z;
//        length = p.z / sin(ofDegToRad(tilt));
//        float r = length*cos(ofDegToRad(tilt));
//        p.x = r*cos(ofDegToRad(pan));
//        p.y = r*sin(ofDegToRad(pan));
//        
//        this->p = p;
//    };
//};
//
//class RectMapper {
//public:
//    vector<Mapper* >  points;
//    ofxQuadWarp warper;
//    float z=-10;
//    RectMapper(){
//        for(int i=0; i<ANCOUR_NUM; i++){
//            points.push_back( new Mapper(z));
//        }
//        //setup warper
//        int w = 640;
//        int h = 480;
//        warper.setSourceRect(ofRectangle(0,0,w,h));
//        warper.setCorner(ofPoint(0,0),0);
//        warper.setCorner(ofPoint(w,0),1);
//        warper.setCorner(ofPoint(w,h),2);
//        warper.setCorner(ofPoint(0,h),3);
//        warper.setup();
//    }
//    void update(){
//        for(int i=0; i<ANCOUR_NUM; i++){
//            ofPoint p = points[i]->p;
//            points[i]->toPoint(p);
//            p.z = 0;
//            warper.setCorner(p,i);
//        }
//    }
//    ofVec4f getCalculatedPoint(ofPoint p){
//        ofPoint matrixed = p*warper.getMatrix();
//        ofVec4f r = ofVec4f(matrixed.x, matrixed.y, 0, 1);
//        r = r/r.w;
//        return r;
//    }
//    void toPanTilt(ofPoint p, float &pan, float &tilt){
//        float length = p.length();
//        pan = ofVec2f(1,0).angle(p);//degree
//        tilt = 90 - ofRadToDeg(atan(length/z));
//    };
//    
//    void addTag(ofxXmlSettings &xml) {
//        for(int i=0; i<ANCOUR_NUM;i++) {
//            xml.addTag("pt");
//            xml.pushTag("pt",i);
//            points[i]->addTag(xml);
//            xml.popTag();
//        }
//    }
//
//    void save(ofxXmlSettings &xml) {
//        for(int i=0; i<ANCOUR_NUM;i++) {
//            xml.pushTag("pt",i);
//            points[i]->save(xml);
//            xml.popTag();
//        }
//    }
//    void load(ofxXmlSettings &xml) {
//        for(int i=0;i<ANCOUR_NUM; i++) {
//            xml.pushTag("pt",i);
//            points[i]->load(xml);
//            xml.popTag();
//        }
//    }
//};
//
//class MovingManager {
//public:
//    vector<RectMapper* > movings;
//    int width = 200;
//    ofxXmlSettings xml;
//    
//    MovingManager() {
//        for(int i=0; i<MOVING_NUM;i++)
//            movings.push_back(new RectMapper());
//        
//        for(int i=0; i<MOVING_NUM; i++){
//            for(int j=0; j<ANCOUR_NUM; j++){
//                ofPoint p = movings[i]->points[j]->gui->getPosition();
//                float height = movings[i]->points[j]->gui->getHeight();
//                movings[i]->points[j]->gui->setPosition((width+10) * i,height * j);
//                movings[i]->points[j]->gui->setWidth(width);
//                
//            }
//        }
//    }
//    void addTag(ofxXmlSettings &xml) {
//        xml.clear();
//        xml.addTag("manager");
//        xml.pushTag("manager");
//        for(int i=0; i<MOVING_NUM;i++){
//            xml.addTag("moving");
//            xml.pushTag("moving",i);
//            movings[i]->addTag(xml);
//            xml.popTag();
//        }
//        xml.popTag();
//    }
//    void save(){
//        if(!xml.tagExists("manager")) addTag(xml);
//        xml.pushTag("manager");
//        for(int i=0; i<MOVING_NUM;i++){
//            xml.pushTag("moving",i);
//            movings[i]->save(xml);
//            xml.popTag();
//        }
//        xml.popTag();
//        xml.saveFile("mapper.xml");
//    }
//    void load() {
//        xml.loadFile("mapper.xml");
//        xml.pushTag("manager");
//        for(int i=0; i<MOVING_NUM;i++) {
//            xml.pushTag("moving",i);
//            movings[i]->load(xml);
//            xml.popTag();
//        }
//        xml.popTag();
//    }
//};