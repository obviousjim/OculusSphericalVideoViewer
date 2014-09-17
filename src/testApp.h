#pragma once

#include "ofMain.h"
#include "ofxOculusDK2.h"
#include "ofxFTGL.h"
//#include "ofxUI.h"

class testApp : public ofBaseApp
{
  public:
	
    enum StereoMode{
        DRAW_STEREO = 0,
        DRAW_LEFT,
        DRAW_RIGHT
    };
    
	void setup();
	void update();
	void draw();
	
    void drawLeftScene();
    void drawRightScene();
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
	
	ofVideoPlayer player;
	
	void drawStringCentered(string string);
	ofxFTGLFont font;
	ofxFTGLFont fontSmall;
	
	void createMeshWithTexture(ofTexture& texture);
	ofImage videoTestPattern;
	ofMesh sphereMeshLeft;
	ofMesh sphereMeshRight;
    float sphereMeshWidth;
    float sphereMeshHeight;
    
	ofVec2f lastMouse;
	ofQuaternion leftCorrection;
	ofQuaternion rightCorrection;
	
	ofCamera cam;
	float sphereScale;
	float converge;
	bool applyInterocular;
	StereoMode showMode; //0 =  stereo, 1 = left, 2 = right
    string filePath;
    void loadFile(string path);
    
    bool showingVideo;
    bool showingStill;
};
