/************************************************************
************************************************************/
#pragma once

/************************************************************
************************************************************/
#include "ofMain.h"
#include "ofxNetwork.h"

/************************************************************
************************************************************/
#define ERROR_MSG(); printf("Error in %s:%d\n", __FILE__, __LINE__);

/************************************************************
************************************************************/
class ofApp : public ofBaseApp{
private:
	/****************************************
	****************************************/
	enum class WindowSize{
		kWidth	= 300,
		kHeight	= 100,
	};
	
	enum class State{
		kRun_,
		kLog_,
	};
	
	State state_ = State::kRun_;
	FILE* fp = NULL;
	
	/********************
	********************/
	int t_from = 0;
	int t_elapsed_sec;
	int t_elapsed_h;
	int t_elapsed_m;
	
	/********************
	********************/
	// const int UDP_BUF_SIZE = 100000;
	
	const int kSendAtOnce_ = 20; // 480
	const int kFps_;
	
	int x_lines_ = 10;
	int y_lines_ = 10;
	
	int num_points_;
	int num_packets_;
	
	vector<ofVec3f> positions;
	
	bool b_1st = true;
	
	ofTrueTypeFont font_M;
	
	/****************************************
	****************************************/
	ofxUDPManager udp_Send_;
	
	/****************************************
	****************************************/
	void SetupUdp();
	void UpdatePosition();
	void PrepAndSendUdp();
	void CalElapsedTime();
	void LogPosition();
	
	
public:
	ofApp(int fps, int x_lines, int y_lines, int SendAtOnce);
	~ofApp();
	
	void setup() override;
	void update() override;
	void draw() override;
	void exit() override;

	void keyPressed(int key) override;
	void keyReleased(int key) override;
	void mouseMoved(int x, int y ) override;
	void mouseDragged(int x, int y, int button) override;
	void mousePressed(int x, int y, int button) override;
	void mouseReleased(int x, int y, int button) override;
	void mouseScrolled(int x, int y, float scrollX, float scrollY) override;
	void mouseEntered(int x, int y) override;
	void mouseExited(int x, int y) override;
	void windowResized(int w, int h) override;
	void dragEvent(ofDragInfo dragInfo) override;
	void gotMessage(ofMessage msg) override;
		
};
