#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxCv.h"

class ofApp : public ofBaseApp{
    
public:
    void setup();
    void update();
    void draw();
    void updateDrawMode();
    void resetPointsAndColors();
    void mousePressed(int x, int y, int button);
    
    
    
    bool bUpdateDrawMode;
    bool bResetPointsAndColors;
    
    int numPoints;
    int drawMode;
    float maxDistance;
    
    int gridX;
    int gridY;
    float waveHeight;
    float baseHeight;
    
    
    
    ofVideoGrabber webcam;
    ofFbo fbo;
    ofxCvColorImage colorImg;
    ofxCvGrayscaleImage grayImage;
    ofxCvGrayscaleImage filtered;
    
    
    ofColor line_color;
    ofColor bg_color;
    ofColor fbo_color;
    
    
    vector <ofVec3f> points;
    vector <ofColor> colors;
    
    ofPath path;
    ofColor stroke_color;
    ofColor fill_color;
    
    ofVideoGrabber cam;
    ofxCv::ObjectFinder finder;
    ofImage sunglasses;
};
