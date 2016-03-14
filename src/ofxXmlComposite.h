#include "ofMain.h"
#include "ofxXmlSettings.h"

class ofxXmlComposite {
public:
    string tagName;
    ofxXmlComposite(string _tagName) : tagName(_tagName){};
    virtual void add(ofxXmlComposite* e)    = 0;
    virtual void remove(ofxXmlComposite* e) = 0;
    virtual void draw() = 0;
    virtual void drawGui() = 0;
    virtual void save(ofxXmlSettings &xml) = 0;
    virtual void load(ofxXmlSettings &xml,int i) = 0;
    virtual void setupGui(ofxDatGui* gui,int i) = 0;
    virtual ofxXmlComposite* getByTag(string findTag,int  &index)=0;
    virtual ofxXmlComposite* getByTag(string findTag){
        int i=0;
        return getByTag(findTag,i);
    }

};


class ofxXmlCompositePack : public ofxXmlComposite {
public:
    ofxXmlCompositePack(string _tagName) : ofxXmlComposite(_tagName){
    }

    vector<ofxXmlComposite* >  elements;
    
    void add(ofxXmlComposite* element){
        elements.push_back(element);
    }
    void remove(ofxXmlComposite* element){
        vector<ofxXmlComposite* >::iterator it = find(elements.begin(), elements.end(), element);
        if(it != elements.end())
            elements.erase(it);
    }
    void draw(){
        for(auto i:elements)
            i->draw();
    };
    void drawGui(){};
    
    void save(ofxXmlSettings &xml){
        int index = xml.addTag(tagName);
            xml.pushTag(tagName,index);
        for(int i=0; i<elements.size();i++) {
            elements[i]->save(xml);
        }
            xml.popTag();
    }
    void load(ofxXmlSettings &xml, int i){
        xml.pushTag(tagName,i);
        for(int i=0; i<elements.size();i++) {
            elements[i]->load(xml,i);
        }
        xml.popTag();
    }
    void setupGui(ofxDatGui* gui,int i){
        for(int i=0;i<elements.size(); i++){
            elements[i]->setupGui(gui,i);
        }
    }

    virtual ofxXmlComposite* getByTag(string findTag){
        int i=0;
        return getByTag(findTag, i);
    };
    virtual ofxXmlComposite* getByTag(string findTag, int &index){
        ofxXmlComposite* r = nullptr;
        if(tagName == findTag) {
            if(index == 0) r = this;
            index--;
        }
        for(int i=0; i<elements.size(); i++) {
            ofxXmlComposite* r2 = elements[i]->getByTag(findTag,index);
            if(r2 !=nullptr) {
                r = r2;
                break;
            }
        }
        return r;
    }
};

class ofxXmlCompositeLeaf : public ofxXmlComposite {
public:
    ofxXmlCompositeLeaf(string tagName) : ofxXmlComposite(tagName) {
    }
    
    void add(ofxXmlComposite* e){};
    void remove(ofxXmlComposite* e){};
    void draw(){};
    void drawGui(){};
    virtual void save(ofxXmlSettings &xml){};
    virtual void load(ofxXmlSettings &xml, int i){};
    virtual void setupGui(ofxDatGui* gui, int i){};
    virtual ofxXmlComposite* getByTag(string findTag, int &index){
        ofxXmlComposite* r = nullptr;
        if(tagName == findTag) {
            if(index == 0) r = this;
            index--;
        }

        return r;
    }
    
};