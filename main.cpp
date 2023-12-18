/************************************************************
************************************************************/
#include "ofMain.h"
#include "ofApp.h"

/************************************************************
************************************************************/

/******************************
******************************/
int main( int argc, char** argv ){
	/********************
	********************/
	//Use ofGLFWWindowSettings for more options like multi-monitor fullscreen
	ofGLWindowSettings settings;
	settings.setSize(1024, 768);
	settings.windowMode = OF_WINDOW; //can also be OF_FULLSCREEN
	
	auto window = ofCreateWindow(settings);
	
	/********************
	********************/
	int fps		= 30;
	int x_lines = 3;
	int y_lines = 3;
	int SendAtOnce = 20;
	
	/********************
	********************/
	printf("---------------------------------\n");
	printf("> parameters\n");
	printf("\t-f      fps(30)\n");
	printf("\t-x      x_lines(3)\n");
	printf("\t-y      y_lines(3)\n");
	printf("\t-s      SendAtOnce(20)\n");
	printf("---------------------------------\n");
	
	for(int i = 1; i < argc; i++){
		if( strcmp(argv[i], "-f") == 0 ){
			if(i+1 < argc) { fps = atoi(argv[i+1]); i++; }
		}else if( strcmp(argv[i], "-x") == 0 ){
			if(i+1 < argc) { x_lines = atoi(argv[i+1]); i++; }
		}else if( strcmp(argv[i], "-y") == 0 ){
			if(i+1 < argc) { y_lines = atoi(argv[i+1]); i++; }
		}else if( strcmp(argv[i], "-s") == 0 ){
			if(i+1 < argc) { SendAtOnce = atoi(argv[i+1]); i++; }
		}
	}
	
	/********************
	********************/
	printf("> parameters\n");
	printf("fps        = %d\n", fps);
	printf("x_lines    = %d\n", x_lines);
	printf("y_lines    = %d\n", y_lines);
	printf("SendAtOnce = %d\n", SendAtOnce);
	fflush(stdout);
	
	/********************
	********************/
	ofRunApp(window, make_shared<ofApp>(fps, x_lines, y_lines, SendAtOnce));
	ofRunMainLoop();
}


