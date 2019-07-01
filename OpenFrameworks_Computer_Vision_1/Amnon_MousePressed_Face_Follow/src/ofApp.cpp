#include "ofApp.h"


using namespace ofxCv;
using namespace cv;


//--------------------------------------------------------------
void ofApp::setup(){
    
    
    stroke_color = ofColor(0);
    gridX = 200;
    gridY = 75;
    waveHeight = 45;
    baseHeight = 6;
    
    
    drawMode = 0;
    bUpdateDrawMode = false;
    
    numPoints = 300;
    maxDistance = 55;
    
    line_color = ofColor(0, 125);
    bg_color = ofColor(255);
    fbo_color = ofColor(0);
    
    
    ofBackground(bg_color);
    ofSetVerticalSync(true);
    finder.setup("haarcascade_frontalface_default.xml");
    finder.setPreset(ObjectFinder::Fast);
    finder.getTracker().setSmoothingRate(.3);
    sunglasses.load("sunglasses.png");
    ofEnableAlphaBlending();
    
    
    
    ofEnableAntiAliasing();
    ofEnableSmoothing();
    ofSetCircleResolution(64);
    ofBackground(bg_color);
    
    webcam.setDesiredFrameRate(12);
    webcam.setDeviceID(0);
    webcam.initGrabber(568, 320);
    
    //fbo.allocate(720, 480, GL_RGBA);
    
    
    
    
    
}
//--------------------------------------------------------------
void ofApp::update(){
    webcam.update();
    if (webcam.isFrameNew()){
        //fbo.begin();
        resetPointsAndColors();
        
        ofClear(0, 0, 0, 0);
        colorImg.setFromPixels(webcam.getPixels());
        filtered = colorImg;
        
        filtered.blurGaussian(9);
        filtered.threshold(70);
        filtered.draw(0, 0);
        //fbo.end();
        
        finder.update(webcam);
        
    }
    
    
    if(bUpdateDrawMode){
        updateDrawMode();
    }
    
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    
    
    
    if (filtered.getHeight() == 0) return;
    
    switch(drawMode){
        case 0: {
    ofFill();
    for(int i = 0; i < points.size(); i++){
        ofSetColor(colors[i]);
        ofVec3f p = points[i];
        ofCircle(p.x, p.y, p.z);
    }
    
    // draw lines between the points within a certain distance of each other
            
    ofNoFill();
    ofSetColor(line_color);
    ofSetLineWidth(0.5);
    for(int i = 0; i < points.size(); i++){
        ofVec3f p = points[i];
        for(int j = i + 1; j < points.size(); j++){
            ofVec3f q = points[j];
            if(ofDist(p.x, p.y, q.x, q.y) < maxDistance){
                ofLine(p.x, p.y, q.x, q.y);
            }
        }
    }
            
            break;
        }
            
            
        case 1: {
            
            float w = float(ofGetWidth()) / gridX;
            float h = float(ofGetHeight()) / gridY;
            ofTranslate(w / 2, h / 2);
            float fc = ofGetElapsedTimef() * 0.35;
            // for each 'row'
            for(int y = 0; y < gridY; y++){
                bool continuous = false;
                // go over all the 'columns'
                for(int x = 0; x < gridX; x++){
                    // for each point, determine it's position in the grid
                    float vx = x * w;
                    float vy = y * h;
                    
                    bool inText = (filtered.getPixels().getColor((int)vx, (int)vy) == fbo_color);
                    if(inText){
                        if(!continuous){
                            // when entering the text
                            continuous = true;
                            path = ofPath();
                            fill_color.setHsb(fmod(vx + 2 * vy + ofGetFrameNum(), 255), 191.25, 216.75);
                            path.setFillColor(fill_color);
                            path.setStrokeColor(stroke_color);
                            path.setStrokeWidth(1.5);
                            path.moveTo(vx, vy);
                        }
                        // add a curveVertex point which is moved upwards using noise()
                        float n = ofNoise(vx * 0.05 + fc, vy * 0.05, fc);
                        vy -= n * n * waveHeight + baseHeight;
                        path.curveTo(vx, vy);
                    }
                    else{
                        if(continuous){
                            // when exiting the text
                            continuous = false;
                            path.lineTo(vx, vy);
                            path.close();
                            path.draw();
                        }
                    }
                }
            }
            
}
            
           
            
            
}
    
   
    for(int i = 0; i < finder.size(); i++) {
        ofRectangle object = finder.getObjectSmoothed(i);
        sunglasses.setAnchorPercent(.5, .5);
        float scaleAmount = .85 * object.width / sunglasses.getWidth();
        ofPushMatrix();
        ofTranslate(object.x + object.width / 2., object.y + object.height * .42);
        ofScale(scaleAmount, scaleAmount);
        sunglasses.draw(0, 0);
        ofPopMatrix();
        ofPushMatrix();
        ofTranslate(object.getPosition());
        //ofDrawBitmapStringHighlight(ofToString(finder.getLabel(i)), 0, 0);
        //ofDrawLine(ofVec2f(), toOf(finder.getVelocity(i)) * 10);
        ofPopMatrix();
    }
    
    
}


//--------------------------------------------------------------

void ofApp::resetPointsAndColors(){
    points.clear();
    colors.clear();
    while(points.size() < numPoints){
        float x = ofRandom(ofGetWidth());
        float y = ofRandom(ofGetHeight());
        bool insideThresh = (filtered.getPixels().getColor(x, y) == fbo_color);
        if(insideThresh){
            
            float radius = ofRandom(1) < 0.075 ? ofRandom(10, 37.5) : ofRandom(1.5, 16.6);
            ofVec3f v = ofVec3f(x, y, radius);
            points.push_back(v);
            ofColor c;
            c.setHsb(ofRandom(255), 153, 216.75, 165.75);
            colors.push_back(c);
            
        }
    }
    
}


//--------------------------------------------------------------
void ofApp::updateDrawMode(){
    drawMode = ++drawMode % 2; // move through 3 drawing modes (0, 1, 2)
    ofBackground(bg_color); // clear the screen when changing drawing mode
    bUpdateDrawMode = false;
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    bUpdateDrawMode = true;
}











