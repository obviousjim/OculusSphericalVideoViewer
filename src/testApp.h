#pragma once

#include "ofMain.h"
#include "ofxOculusDK2.h"
#include "ofxFTGL.h"
#include "ofxUI.h"

#ifdef TARGET_WIN32
#include "ofDirectShowPlayer.h"
#endif

class testApp : public ofBaseApp
{
  public:
	
	void setup();
	void update();
	void draw();
	
	void drawScene(ofMesh& mesh);
	
	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);
	
	void loadSettings();
	void saveSettings();
	
	ofxOculusDK2 oculusRift;
	
	#ifdef TARGET_WIN32
	ofDirectShowPlayer player;
	#else
	ofVideoPlayer player;
	#endif
	
	void drawStringCentered(string string);
	ofxFTGLFont font;
	ofxFTGLFont fontSmall;
	
	void createMeshWithTexture(ofTexture& texture);
	ofImage videoTestPattern;
	ofTexture videoTexture;
	ofMesh sphereMeshLeft;
	ofMesh sphereMeshRight;

	ofVec2f lastMouse;
	ofQuaternion leftCorrection;
	ofQuaternion rightCorrection;
	
	ofCamera cam;
	float sphereScale;
	float converge;
	bool applyInterocular;
//	ofVec3f globalRotate;
//	ofVec3f eyeRotate;
	
};
