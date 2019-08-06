#include "ofApp.h"

using namespace ofxCv;
using namespace cv;
//--------------------------------------------------------------
void ofApp::setup(){

    cam.setDeviceID(0);
    cam.setup(320, 240);
    
    k=5;
    
    ofBackground(50);
}

//--------------------------------------------------------------
void ofApp::update(){

    cam.update();
    
    if (cam.isFrameNew()) {
        
        // copy to src
        cv::Mat src;
        ofxCv::copy(cam, src);
        
        // reshape to 1d and convert to 32f
        int total = src.rows * src.cols;
        cv::Mat data = src.reshape(1, total);
        data.convertTo(data, CV_32F);
        
        // run kmeans
        cv::Mat colors;
        std::vector<int> labels;
        cv::kmeans(data, k, labels, cv::TermCriteria(cv::TermCriteria::COUNT + cv::TermCriteria::EPS, 10, 1.0), 3, cv::KmeansFlags::KMEANS_PP_CENTERS, colors);
        
        // remap color
        for(int i=0; i<total; i++) {
            data.at<float>(i, 0) = colors.at<float>(labels[i], 0);
            data.at<float>(i, 1) = colors.at<float>(labels[i], 1);
            data.at<float>(i, 2) = colors.at<float>(labels[i], 2);
        }
        
        // reshape back
        cv::Mat output = data.reshape(3, src.rows);
        output.convertTo(output, CV_8U);
        
        // copy to ofImage
        ofxCv::copy(output, aa);
        aa.update();
        
        // update color palette
        palette.clear();
        
        for(int i=0; i<k; i++) {
            auto r = colors.at<float>(i, 0);
            auto g = colors.at<float>(i, 1);
            auto b = colors.at<float>(i, 2);
            palette.push_back(ofColor(r, g, b));
        }
        
        // sort on brightness
        ofSort(palette, [](ofColor &a, ofColor &b) -> bool {
            return a.getBrightness() < b.getBrightness();
        });
        
        //bCapture = false;
    }
}

//--------------------------------------------------------------
void ofApp::draw(){

    // 도미넌트 컬러들 사각형 표시
    for(int i=0; i<palette.size(); i++) {
        float x = cam.getWidth()+20;
        float y = i * 50;
        ofFill();
        ofSetColor(palette[i]);
        ofDrawRectangle(x, y, 100, 50);
    }
    
    ofSetColor(255);
    cam.draw(0, 0);
    aa.draw(0, cam.getHeight());
    
    ofDrawBitmapStringHighlight("Frame/sec=" +
            ofToString(ofGetFrameRate(), 0) +", "+
            ofToString(k)+"-Dominant Colors", 20, ofGetHeight()-20);

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

    if (key == OF_KEY_UP) {
        k ++;
    }
    if (key == OF_KEY_DOWN) {
        k --;
        if (k <= 0) k = 1;
    }

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
