/************************************************************
************************************************************/
#include "ofApp.h"
#include <algorithm> // to use std::min

/************************************************************
************************************************************/

/******************************
******************************/
ofApp::ofApp(int fps, int x_lines, int y_lines, int SendAtOnce)
: kFps_(fps)
, x_lines_(x_lines)
, y_lines_(y_lines)
, kSendAtOnce_(SendAtOnce)
{
	font_M.load("font/RictyDiminished-Regular.ttf", 30/* font size in pixels */, true/* _bAntiAliased */, true/* _bFullCharacterSet */, false/* makeContours */, 0.3f/* simplifyAmt */, 72/* dpi */);
	
	num_points_		= x_lines_ * y_lines_;
	num_packets_	= num_points_ / kSendAtOnce_;
	if(num_points_ % kSendAtOnce_ != 0) num_packets_++;
	
	positions.resize(num_points_);
}

/******************************
******************************/
ofApp::~ofApp(){
	if(fp)	fclose(fp);
}

/******************************
******************************/
void ofApp::setup(){
	/********************
	********************/
	ofSetWindowTitle("dummy LiDAR");
	
	ofSetWindowShape( static_cast<int>(WindowSize::kWidth), static_cast<int>(WindowSize::kHeight) );
	ofSetVerticalSync(false);	// trueとどっちがいいんだろう？
	ofSetFrameRate(kFps_);
	
	ofSetEscapeQuitsApp(false);
	
	/********************
	********************/
	ofEnableAntiAliasing();
	ofEnableBlendMode(OF_BLENDMODE_ALPHA); // OF_BLENDMODE_DISABLED, OF_BLENDMODE_ALPHA, OF_BLENDMODE_ADD
	
	/********************
	********************/
	SetupUdp();
}

/******************************
******************************/
void ofApp::SetupUdp(){
	/********************
	********************/
	{
		ofxUDPSettings settings;
		settings.sendTo("127.0.0.1", 12345);
		// settings.sendTo("10.0.0.10", 12345);
		settings.blocking = false;
		
		udp_Send_.Setup(settings);
	}
}

/******************************
******************************/
void ofApp::update(){
	UpdatePosition();
	PrepAndSendUdp();
	
	if(state_ == State::kLog_){
		CalElapsedTime();
		LogPosition();
	}
}

/******************************
******************************/
void ofApp::UpdatePosition(){
	float time_sec = (float)ofGetElapsedTimeMillis() / 1000;
	
	int ofs = 0;
	for(int i = 0; i < x_lines_; i++){
		float x = i - x_lines_ * 0.5f + 0.5f;
		for(int j = 0; j < y_lines_; j++){
			float z = j - y_lines_ * 0.5f + 0.5f;
			float y = sin(sqrt(x * x + z * z) * 0.4f - time_sec * 2);
			
			// positions[ofs] = ofVec3f(x, y, z);
			positions[ofs].set(x, y, z);
			
			ofs++;
		}
	}
}

/******************************
******************************/
void ofApp::PrepAndSendUdp(){
	const int BUF_SIZE = 100;
	
	for(int ofs = 0, id = 0; ofs < (int)positions.size(); ofs += kSendAtOnce_, id++){
		int count = std::min(kSendAtOnce_, (int)positions.size() - ofs); // positions.size() は unsigned なので、castしないと、build通らない
		
		string message = "/pos,";
		
		{
			char buf[BUF_SIZE];
			snprintf(buf, std::size(buf), "%d,%d,%d,%d,%d,", 0/* grop_id */, num_points_, num_packets_, id, ofs);
			message += buf;
		}
		
		for(int i = 0; i < count; i++){
			char buf[BUF_SIZE];
			snprintf(buf, std::size(buf), "%.2f,%.2f,%.2f", positions[i + ofs].x, positions[i + ofs].y, positions[i + ofs].z );
			message += buf;
			
			if(i < count - 1)	{ message += ","; }
		}
		
		udp_Send_.Send(message.c_str(), message.length());
	}
}

/******************************
******************************/
void ofApp::LogPosition(){
	for(int i = 0; i < num_points_; i++){
		string message = "";
		
		char buf[100];
		snprintf(buf, std::size(buf), "%.2f,%.2f,%.2f", positions[i].x, positions[i].y, positions[i].z);
		message += buf;
		
		if(i < num_points_ - 1)	{ message += ","; }
		
		fprintf(fp, "%s", message.c_str());
	}
	
	fprintf(fp, "\n");
}

/******************************
******************************/
void ofApp::CalElapsedTime(){
	t_elapsed_sec = (ofGetElapsedTimeMillis() - t_from) / 1000;
	
	t_elapsed_h		=  t_elapsed_sec / (60 * 60);
	t_elapsed_sec	-= t_elapsed_h * (60 * 60);
	
	t_elapsed_m		=  t_elapsed_sec / 60;
	t_elapsed_sec	-= t_elapsed_m * 60;
}


/******************************
******************************/
void ofApp::draw(){
	/********************
	********************/
	ofBackground(0);
	
	/********************
	********************/
	ofSetColor(255);
	
	switch(state_){
		case State::kRun_:
			font_M.drawString("Run", 34, 43);
			break;
			
		case State::kLog_:
		{
			font_M.drawString("Log", 34, 43);
			
			char buf[500];
			snprintf(buf, std::size(buf), "%02d:%02d:%02d", t_elapsed_h, t_elapsed_m, t_elapsed_sec);
			font_M.drawString(buf, 136, 43);
		}
			break;
	}
	
	char buf[500];
	snprintf(buf, std::size(buf), "%02d fps", (int)ofGetFrameRate());
	font_M.drawString(buf, 136, 73);
}

/******************************
******************************/
void ofApp::exit(){
}

/******************************
******************************/
void ofApp::keyPressed(int key){
	switch(key){
		case ' ':
			if(state_ == State::kRun_){
				state_ = State::kLog_;
				
				t_from = ofGetElapsedTimeMillis();
				
				fp = fopen("../../../data/Log/Log.csv", "w");
				if(!fp)	{ ERROR_MSG(); std::exit(1); }
				
				char buf[100];
				snprintf(buf, std::size(buf), "fps,%d\n", kFps_);
				fprintf(fp, "%s", buf);
				
			}else{
				state_ = State::kRun_;
				
				fclose(fp);
				fp = NULL;
			}
			
			break;
	}
	
}

/******************************
******************************/
void ofApp::keyReleased(int key){

}

/******************************
******************************/
void ofApp::mouseMoved(int x, int y ){

}

/******************************
******************************/
void ofApp::mouseDragged(int x, int y, int button){

}

/******************************
******************************/
void ofApp::mousePressed(int x, int y, int button){

}

/******************************
******************************/
void ofApp::mouseReleased(int x, int y, int button){

}

/******************************
******************************/
void ofApp::mouseScrolled(int x, int y, float scrollX, float scrollY){

}

/******************************
******************************/
void ofApp::mouseEntered(int x, int y){

}

/******************************
******************************/
void ofApp::mouseExited(int x, int y){

}

/******************************
******************************/
void ofApp::windowResized(int w, int h){

}

/******************************
******************************/
void ofApp::gotMessage(ofMessage msg){

}

/******************************
******************************/
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
