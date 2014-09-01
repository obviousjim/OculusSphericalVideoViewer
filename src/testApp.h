#pragma once

#include "ofMain.h"
#include "ofxOculusDK2.h"
#include "ofxFTGL.h"

#ifdef TARGET_WIN32
#include "ofDirectShowPlayer.h"
#else
#include "ofAVFoundationPlayer.h"
#endif

class testApp : public ofBaseApp
{
  public:
	
	void setup();
	void update();
	void draw();
	
	void drawScene();
	
	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);

	ofxOculusDK2 oculusRift;
	
	#ifdef TARGET_WIN32
	ofDirectShowPlayer player;
	#else
	ofAVFoundationPlayer player;
	#endif
	
	void drawStringCentered(string string);
	ofxFTGLFont font;
	ofxFTGLFont fontSmall;
	
	void createMeshWithTexture(ofTexture& texture);
	ofImage videoTestPattern;
	ofTexture videoTexture;
	ofMesh sphereMesh;
	ofCamera cam;
    
};
