#include "testApp.h"

#define STRINGIZE2(s) #s
#define STRINGIZE(s) STRINGIZE2(s)

//--------------------------------------------------------------
void testApp::setup() {
	
	ofBackground(0);
	ofSetVerticalSync(true);

	//look in the bundle!
	#ifdef TARGET_OSX
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
		player.loadMovie( STRINGIZE(VIDEO_FILE_NAME) );
		player.setLoopState(OF_LOOP_NORMAL);
		player.play();
		ofToggleFullscreen();
		#else
		videoTestPattern.loadImage("video_test_pattern.png");
		createMeshWithTexture(videoTestPattern.getTextureReference());
		#endif
		
		
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
			videoTexture.allocate(player.getWidth(),player.getHeight(), GL_RGB);
			createMeshWithTexture(videoTexture);
		}

		videoTexture.loadData(player.getPixelsRef());
	}
}

//--------------------------------------------------------------
void testApp::createMeshWithTexture(ofTexture& texture){
	
	ofSpherePrimitive p = ofSpherePrimitive(100.0,60);
	sphereMesh = p.getMesh();
	
	for(int i = 0; i < sphereMesh.getNumVertices(); i++){
		ofVec2f texCoord = sphereMesh.getTexCoord(i);
		texCoord.x *= texture.getWidth();
		texCoord.y  = (1.0 - texCoord.y) * texture.getHeight();
		sphereMesh.setTexCoord(i, texCoord);
	}
	
}

//--------------------------------------------------------------
void testApp::draw()
{

	if(oculusRift.isSetup()){

		ofPushStyle();
		string instructions = "'f' toggle full screen";
		int width = fontSmall.stringWidth(instructions);
		ofSetColor(255*.4);
		fontSmall.drawString(instructions,oculusRift.getOculusViewport().getWidth() - width/2, 40);
		ofPopStyle();
		
		if(!videoTexture.isAllocated()){
			int overlayWidth  = 512;
			int overlayHeight = 512;
			string loadText = "Drag & Drop a video";
			width = fontSmall.stringWidth(loadText);
			int height = fontSmall.stringHeight(loadText);
			//draw the instructions into the viewport
			oculusRift.beginOverlay(-250, overlayWidth,overlayHeight);
			fontSmall.drawString(loadText,
								 overlayWidth/2 - width/2,
								 overlayHeight/2 - height/2);
			oculusRift.endOverlay();
		}
		
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
	ofPushMatrix();
//	ofScale(1,-1,1);
	
	if(videoTexture.isAllocated()){
		videoTexture.bind();
		sphereMesh.draw();
		videoTexture.unbind();
	}
	else{
		videoTestPattern.getTextureReference().bind();
		sphereMesh.draw();
		videoTestPattern.getTextureReference().unbind();
	}

	//debug wireframe for fun
//	ofDisableDepthTest();
//	sphereMesh.drawWireframe();
//	ofEnableDepthTest();
	
	ofPopMatrix();
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
	if(player.loadMovie( dragInfo.files[0] )){
		player.play();
		player.setLoopState(OF_LOOP_NORMAL);
	}
	else{
		ofSystemAlertDialog("Video failed to load.");
	}
	#endif
	
}
