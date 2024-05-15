#pragma once

#include "ofMain.h"
#include "ofxPd.h"
#include "ofxOsc.h"

using namespace pd;

// listening port
#define PORT 50010


// max number of strings to display
#define NUM_MSG_STRINGS 22

// demonstrates receiving and processing OSC messages with an ofxOscReceiver,
// use in conjunction with the oscSenderExample
class ofApp : public ofBaseApp{
    public:

        void setup();
        void update();
        void draw();

        void keyPressed(int key);
        void keyReleased(int key);
        void mouseMoved(int x, int y);
        void mouseDragged(int x, int y, int button);
        void mousePressed(int x, int y, int button);
        void mouseReleased(int x, int y, int button);
        void mouseEntered(int x, int y);
        void mouseExited(int x, int y);
        void windowResized(int w, int h);
        void dragEvent(ofDragInfo dragInfo);
        void gotMessage(ofMessage msg);
        void moving_average(int x,int y,int z);
        void loop();
       

        ofTrueTypeFont font;
        ofxOscReceiver receiver;

        int currentMsgString;
        string msgStrings[NUM_MSG_STRINGS];
        float timers[NUM_MSG_STRINGS];
        int coordinate_move = 0;

        float mouseXf = 0;
        float mouseYf = 0;
        int mouseButtonInt = 0;
        string mouseButtonState = "";
        vector<string> positions = {"0", "0", "0"};
        vector<int> x15 = {0, 0};
        vector<int> y15 = {0, 0};
        vector<int> z15 = {0, 0};

        vector<int> x16 = {0, 0};
        vector<int> y16 = {0, 0};
        vector<int> z16 = {0, 0};
    
        vector<float> shader_x15 = {0.0, 0.0};
        vector<float> shader_y15 = {0.0, 0.0};
    
        vector<float> shader_x16 = {0.0, 0.0};
        vector<float> shader_y16 = {0.0, 0.0};

    
        float adjustMeshRadius;
    
        float posX, posY;
        float goalX, goalY;
        float velX, velY;
    
        
        int center;
        int es1, es2;
    

        ofxPd pd;
        ofImage receivedImage;
        ofMesh mesh;
        ofSpherePrimitive sphere;
        ofPlanePrimitive plane;
        ofEasyCam cam;
        ofShader shader;
        ofTexture texA;
        
    
    // do something
    void playTone(int pitch);
    
    // audio callbacks
    void audioReceived(float * input, int bufferSize, int nChannels);
    void audioRequested(float * output, int bufferSize, int nChannels);
    
    // pd message receiver callbacks
    void print(const std::string& message);

    
        vector<Patch> instances;
    
    

    
        

        
};
