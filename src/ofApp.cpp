#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofSetVerticalSync(true);
    ofSetWindowTitle("Shader");
    ofSetFrameRate(60); // run at 60 fps
    ofSetVerticalSync(true);

    // listen on the given port
    ofLog() << "listening for osc messages on port " << PORT;
    receiver.setup(PORT);
    cout << ofFilePath::getCurrentWorkingDirectory() << endl;
    
    // the number of libpd ticks per buffer,
    // used to compute the audio buffer len: tpb * blocksize (always 64)
#ifdef TARGET_LINUX_ARM
    // longer latency for Raspberry PI
    int ticksPerBuffer = 32; // 32 * 64 = buffer len of 2048
    int numInputs = 0; // no built in mic
#else
    int ticksPerBuffer = 8; // 8 * 64 = buffer len of 512
    int numInputs = 1;
#endif
    
    // ofのサウンド初期化（出力チャンネル数, 入力チャンネル数, 呼び出し元, サンプリング周波数, ティックごとのバッファサイズ, バッファ数）
    ofSoundStreamSetup(2, numInputs, this, 44100, ofxPd::blockSize()*ticksPerBuffer, 3);
    
    // pdのサウンド初期化（出力チャンネル数, 入力チャンネル数, サンプリング周波数, ティックごとのバッファサイズ, キューの有無）
    if(!pd.init(2, numInputs, 44100, ticksPerBuffer, false)) {
        OF_EXIT_APP(1);
    }
    
    // pdのオーディオ処理をスタート
    pd.start();

    // pdフォルダ内のPureDateを開く
    Patch patch = pd.openPatch("Pd/of_test_4.pd");
    cout << patch << endl;

    
    //shaderの読み込み
   // shader.load("", "Shader/shader.frag");
    
    


}

//--------------------------------------------------------------
void ofApp::update(){
    
    pd.sendFloat("fromOF", 1);

    // hide old messages
    for(int i = 0; i < NUM_MSG_STRINGS; i++){
        if(timers[i] < ofGetElapsedTimef()){
            msgStrings[i] = "";
        }
    }
     
    // check for waiting messages
    while(receiver.hasWaitingMessages()){

        // get the next message
        ofxOscMessage m;
        receiver.getNextMessage(m);
    
         
        
        if(m.getAddress() == "/positions"){
            if(m.getArgType(0) == OFXOSC_TYPE_STRING){
                vector<string> positions = ofSplitString(m.getArgAsString(0), ",");
                
               // ofLog() << positions[0] << "," << positions[1] << "," << positions[2] << "," << positions[3];
            }
        }
        // check for an image being sent
        // note: the size of the image depends greatly on your network buffer
        // sizes, if an image is too big the message won't come through
        else if(m.getAddress() == "/image"){
            ofBuffer buffer = m.getArgAsBlob(0);
            receivedImage.load(buffer);
        }
        else{

            // unrecognized message: display on the bottom of the screen
            string msgString;
            msgString = m.getAddress();
            msgString += ":";
            
            
            for(size_t i = 0; i < m.getNumArgs(); i++){

                // get the argument type
                msgString += " ";
                msgString += m.getArgTypeName(i);
                msgString += ":";

                // display the argument - make sure we get the right type
                if(m.getArgType(i) == OFXOSC_TYPE_INT32){
                    msgString += ofToString(m.getArgAsInt32(i));
                }
                else if(m.getArgType(i) == OFXOSC_TYPE_FLOAT){
                    msgString += ofToString(m.getArgAsFloat(i));
                }
                else if(m.getArgType(i) == OFXOSC_TYPE_STRING){
                    msgString += m.getArgAsString(i);
                }
                else{
                    msgString += "unhandled argument type " + m.getArgTypeName(i);
                }
            }

            // add to the list of strings to display
            msgStrings[currentMsgString] = msgString;
            timers[currentMsgString] = ofGetElapsedTimef() + 5.0f;
            currentMsgString = (currentMsgString + 1) % NUM_MSG_STRINGS;
            

            vector<string> strings1 = ofSplitString(msgString, "[");//括弧を中心として文字列を分けて配列に入れる
            vector<string> strings2 = ofSplitString(strings1[1], "]");
            positions = ofSplitString(strings2[0], ",");
            //ofLog()<< ofToInt(positions[0]) << ", " << ofToInt(positions[1]) << ", " << ofToInt(positions[2]) << " : " << ofToInt(positions[3]) << " , " << ofToInt(positions[4])<< " , " << ofToInt(positions[5]);
            
            //画面中央からの手の位置(x/y)の距離
            center = ofGetWidth()/2, ofGetHeight()/2;
            es1 = ofDist(x15[1], y15[1], center, center);
            es2 = ofDist(x16[1], y16[1], center, center);
            
            pd.sendFloat("es1", es1);
            pd.sendFloat("es2", es2);
            
            
            
            //for(int i = 0; i < positions.size(); i++){
                
                x15[1] = 0.9 * x15[0] + 0.2 * ofToInt(positions[0]);
                /* フィルタ出力を使った処理 */
                x15[0] = x15[1];
                pd.sendFloat("x15", x15[0]/2);

               
                y15[1] = 0.9 * y15[0] + 0.2 * ofToInt(positions[1]);
                /* フィルタ出力を使った処理 */
                y15[0] = y15[1];
                pd.sendFloat("y15", y15[0]/2);

                
                z15[1] = 0.9 * z15[0] + 0.2 * ofToInt(positions[2]);
                /* フィルタ出力を使った処理 */
                z15[0] = z15[1];
                pd.sendFloat("z15", z15[0]/2);

                
             
                x16[1] = 0.9 * x16[0] + 0.2 * ofToInt(positions[3]);
                /* フィルタ出力を使った処理 */
                x16[0] = x16[1];
                pd.sendFloat("x16", x16[0]/2);

        
                y16[1] = 0.9 * y16[0] + 0.2 * ofToInt(positions[4]);
                /* フィルタ出力を使った処理 */
                y16[0] = y16[1];
                pd.sendFloat("y16", y16[0]/2);

                
             
                z16[1] = 0.9 * z16[0] + 0.2 * ofToInt(positions[5]);
                /* フィルタ出力を使った処理 */
                z16[0] = z16[1];
                pd.sendFloat("z16", z16[0]/2);

                ofLog()<< x15[0]/2 << ", " << y15[0]/2 << ", " << z15[0]/2 << " : " << x16[0]/2 << " , " << y16[0]/2 << " , " << z16[0]/2;
                
        //}
            
            // clear the next line
            msgStrings[currentMsgString] = "";
            

            
        }
    }

}

//--------------------------------------------------------------
void ofApp::draw(){
    
    
    
//    ofPushMatrix();

//     ofTranslate( ofGetWidth()/4.0, ofGetHeight()/4.0, 0 );

        ofEnableDepthTest();
        ofSetColor(x15[0]/3,y15[0]/3,z15[0] * -1);
        ofDrawCircle(x15[0]/2,y15[0]/2,y15[0] * -2,y15[0] );
        ofSetColor(x16[0]/4,y16[0]/1.5,z16[0] * -1);
        ofDrawCircle(x16[0]/1.5,y16[0],y16[0] * -2,y16[0]);


        ofSetColor(0);
        ofSetColor(255);
        ofDrawBitmapString("Some bubbles.",85,90);

//    ofPopMatrix();



    
    
    
//    shader_x15[0] = ofToFloat(positions[0]);
//    shader_y15[0] = ofToFloat(positions[1]);
//    shader_x16[0] = ofToFloat(positions[3]);
//    shader_y16[0] = ofToFloat(positions[4]);
//    
//
//    shader.begin();
//    // send uniform variables to shader (Elapsed time in seconds)
//    shader.setUniform1f("u_time", ofGetElapsedTimef());
//    // send uniform variables to shader (resolution)
//    shader.setUniform2f("u_resolution", ofGetWidth(), ofGetHeight());
//    //座標を送信
//    shader.setUniform1f("u_coordinates", shader_x15[0]/2000 * 256/*,shader_y15[0]*/);
//    //shader.setUniform2f("u_coordinates", shader_x16[0],shader_y16[0]);
//    ofRect(0,0, ofGetWidth(), ofGetHeight());
//    /*mesh.draw();*/
//    shader.end();

    


}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

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
/**
oFサウンドストリームの音入力を受け取ってPdへ（オーディオを入力）
 
@param input サウンドインプット
@param bufferSize バッファのサイズ
@param nChannels チャンネル数
 
     */
void ofApp::audioReceived(float * input, int bufferSize, int nChannels) {
        pd.audioIn(input, bufferSize, nChannels);
    }

    //--------------------------------------------------------------
    /**
     oFサウンドストリームの音出力を受け取ってPdへ（オーディオを出力）

     @param output 出力するサウンド
     @param bufferSize バッファのサイズ
     @param nChannels チャンネル数
     */
    void ofApp::audioRequested(float * output, int bufferSize, int nChannels) {
        pd.audioOut(output, bufferSize, nChannels);

}
