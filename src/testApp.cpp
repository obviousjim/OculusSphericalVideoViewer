#include "testApp.h"

#define STRINGIZE2(s) #s
#define STRINGIZE(s) STRINGIZE2(s)

//--------------------------------------------------------------
void testApp::setup() {
	
	ofBackground(0);
	ofSetVerticalSync(true);

	//look in the bundle!
	#if defined(VIDEO_FILE_NAME) && defined(TARGET_OSX)
	ofDisableDataPath();
    CFBundleRef mainBundle = CFBundleGetMainBundle();
    CFURLRef resourcesURL = CFBundleCopyResourcesDirectoryURL(mainBundle);
    char path[PATH_MAX];
    CFURLGetFileSystemRepresentation(resourcesURL, TRUE, (UInt8 *)path, PATH_MAX);
    CFRelease(resourcesURL);
    chdir(path);
	#endif
	
	#ifndef TARGET_WIN32
	player.setUseTexture(false);
	#endif

	font.loadFont("HelveticaNeueBold.ttf", 28);
	fontSmall.loadFont("HelveticaNeueBold.ttf", 10);
	
	oculusRift.baseCamera = &cam;
	oculusRift.setup();
	
	if(oculusRift.isSetup()){
		
		//enable mouse;
		cam.begin();
		cam.end();
		cam.setPosition(0,0,0);
		
		//if this is set then the app looks for a video right away
		#ifdef VIDEO_FILE_NAME
		player.loadMovie(STRINGIZE(VIDEO_FILE_NAME));
		player.play();
		#endif
		
		ofToggleFullscreen();
	}
	
}

//--------------------------------------------------------------
void testApp::update()
{
	if(!oculusRift.isSetup()){
		return;
	}
	
	player.update();
	if(player.isFrameNew()){
		
		if(!videoTexture.isAllocated() ||
		   videoTexture.getWidth() != player.getWidth() ||
		   videoTexture.getHeight() != player.getHeight())
		{
			ofSpherePrimitive p = ofSpherePrimitive(100.0,20);
			sphereMesh = p.getMesh();
			
			for(int i = 0; i < sphereMesh.getNumVertices(); i++){
				sphereMesh.setTexCoord(i,sphereMesh.getTexCoord(i) * ofVec2f(player.getWidth(),player.getHeight()));
			}
			
			videoTexture.allocate(player.getWidth(),player.getHeight(), GL_RGB);
		}

		videoTexture.loadData(player.getPixelsRef());
	}
}

//--------------------------------------------------------------
void testApp::draw()
{

	if(oculusRift.isSetup()){

        ofSetColor(255);

		glEnable(GL_DEPTH_TEST);
		
		oculusRift.beginLeftEye();
		drawScene();
		oculusRift.endLeftEye();
		
		oculusRift.beginRightEye();
		drawScene();
		oculusRift.endRightEye();
		
		oculusRift.draw();
		
		glDisable(GL_DEPTH_TEST);
		
		ofPushStyle();
		string instructions = "'f' toggle full screen";
		int width = fontSmall.stringWidth(instructions);
		ofSetColor(255*.4);
		fontSmall.drawString(instructions,oculusRift.getOculusViewport().getWidth() - width/2, 40);
		ofPopStyle();
    }
	else{
		ofPushStyle();
		ofSetColor(255*.4);
		drawStringCentered("Rift not found! Plug it in and restart the app.");
		ofPopStyle();
	}

}

//--------------------------------------------------------------
void testApp::drawStringCentered(string str){
	int width = font.stringWidth(str);
	int height = font.stringHeight(str);
	font.drawString(str, ofGetWidth()/2 - width/2, ofGetHeight()/2 - height/2);
}

//--------------------------------------------------------------
void testApp::drawScene()
{
	ofPushStyle();
	ofScale(1,-1,1);
	
	videoTexture.bind();
	sphereMesh.draw();
	videoTexture.unbind();
	
//	ofDisableDepthTest();
//	sphereMesh.drawWireframe();
//	ofEnableDepthTest();
	
	ofPopStyle();
}

//--------------------------------------------------------------
void testApp::keyPressed(int key)
{
	if( key == 'f' ){
		//gotta toggle full screen for it to be right
		ofToggleFullscreen();
	}

}

//--------------------------------------------------------------
void testApp::keyReleased(int key)
{

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y)
{
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button)
{
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button)
{
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button)
{

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h)
{

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg)
{

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo)
{
	//this is for the standalone player
	#ifndef VIDEO_FILE_NAME
	player.loadMovie( dragInfo.files[0] );
	player.play();
	#endif
	
}
