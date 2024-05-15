#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main( ){
    ofGLFWWindowSettings settings;
    settings.setGLVersion(2, 1);
    settings.setSize(1280, 720);
    ofCreateWindow(settings);
    ofRunApp(new ofApp());

}
