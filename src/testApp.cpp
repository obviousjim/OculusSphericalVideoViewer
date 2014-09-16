#include "testApp.h"

#define STRINGIZE2(s) #s
#define STRINGIZE(s) STRINGIZE2(s)

//--------------------------------------------------------------
void testApp::setup() {
	
	ofBackground(0);
	ofSetVerticalSync(true);

	player.loadMovie( "video_test.mov" );
	ofToggleFullscreen();
	
	//look in the bundle!
//	#ifdef TARGET_OSX
//	ofDisableDataPath();
//  CFBundleRef mainBundle = CFBundleGetMainBundle();
//  CFURLRef resourcesURL = CFBundleCopyResourcesDirectoryURL(mainBundle);
//  char path[PATH_MAX];
//  CFURLGetFileSystemRepresentation(resourcesURL, TRUE, (UInt8 *)path, PATH_MAX);
//  CFRelease(resourcesURL);
//  chdir(path);
//	#endif
    
    sphereMeshWidth = 0;
    sphereMeshHeight = 0;
	showMode = DRAW_STEREO;
    
	#ifndef TARGET_WIN32
	player.setUseTexture(false);
	#endif

	font.loadFont("font/HelveticaNeueBold.ttf", 28);
	fontSmall.loadFont("font/HelveticaNeueBold.ttf", 10);
	
	oculusRift.baseCamera = &cam;
	oculusRift.applyTranslation = false; //don't move the camers
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
void testApp::loadSettings(){
	ofBuffer b;
	if(ofFile("settings.txt").exists()){
		
		b = ofBufferFromFile("settings.txt");
		
		leftCorrection.set(ofToFloat(b.getNextLine()),
						   ofToFloat(b.getNextLine()),
						   ofToFloat(b.getNextLine()),
						   ofToFloat(b.getNextLine()));

		rightCorrection.set(ofToFloat(b.getNextLine()),
							ofToFloat(b.getNextLine()),
							ofToFloat(b.getNextLine()),
							ofToFloat(b.getNextLine()));
		
		converge = ofToFloat( b.getNextLine() );
	}
		
}

//--------------------------------------------------------------
void testApp::saveSettings(){
	ofBuffer b;
	
	b.append(ofToString(leftCorrection.x())+"\n");
	b.append(ofToString(leftCorrection.y())+"\n");
	b.append(ofToString(leftCorrection.z())+"\n");
	b.append(ofToString(leftCorrection.w())+"\n");

	b.append(ofToString(rightCorrection.x())+"\n");
	b.append(ofToString(rightCorrection.y())+"\n");
	b.append(ofToString(rightCorrection.z())+"\n");
	b.append(ofToString(rightCorrection.w())+"\n");
	
	b.append(ofToString(converge)+"\n");
	
	ofBufferToFile("settings.txt", b);
}

//--------------------------------------------------------------
void testApp::update()
{
	if(!oculusRift.isSetup()){
		return;
	}
	if(player.isLoaded()){
        player.update();
        if(player.isFrameNew()){
            if(sphereMeshWidth != player.getWidth() ||
               sphereMeshHeight != player.getHeight())
            {
                createMeshWithTexture(player.getTextureReference());
            }

        }
    }
}

//--------------------------------------------------------------
void testApp::createMeshWithTexture(ofTexture& texture){
	
	ofSpherePrimitive p = ofSpherePrimitive(1,60);
	sphereMeshLeft  = p.getMesh();
	sphereMeshRight = p.getMesh();
	
	float halfHeight = texture.getHeight() * .5;
	for(int i = 0; i < sphereMeshLeft.getNumVertices(); i++){
		ofVec2f texCoord = sphereMeshLeft.getTexCoord(i);
		texCoord.x = (1.0 - texCoord.x) * texture.getWidth();
		texCoord.y = (1.0 - texCoord.y) * halfHeight;
		sphereMeshLeft.setTexCoord(i, texCoord);
	}
	
	for(int i = 0; i < sphereMeshRight.getNumVertices(); i++){
		ofVec2f texCoord = sphereMeshRight.getTexCoord(i);
		texCoord.x = (1.0 - texCoord.x) * texture.getWidth();
		texCoord.y = (1.0 - texCoord.y) * halfHeight + halfHeight;
		sphereMeshRight.setTexCoord(i, texCoord);
	}
    
    sphereMeshWidth = texture.getWidth();
    sphereMeshHeight = texture.getHeight();
	
}

//--------------------------------------------------------------
void testApp::draw()
{

	if(oculusRift.isSetup()){

		ofPushStyle();
		string instructions = "'f' toggle full screen";
		int width = fontSmall.stringWidth(instructions);
		ofSetColor(255*.4);
		fontSmall.drawString(instructions, ofGetWidth()/2 - width/2, 40);
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
		
		
		ofEnableDepthTest();
		
		oculusRift.beginLeftEye();
        if(showMode == DRAW_RIGHT){
            drawRightScene();
        }
        else{
            drawLeftScene();
        }
		oculusRift.endLeftEye();
		
		oculusRift.beginRightEye();
        if(showMode == DRAW_LEFT){
            drawLeftScene();
        }
        else{
            drawRightScene();
        }
		oculusRift.endRightEye();
		
		oculusRift.draw();
		
		ofDisableDepthTest();
		
    }
	else{
		ofPushStyle();
		ofSetColor(255*.4);
		drawStringCentered("Rift not found! Plug it in and restart the app.");
		ofPopStyle();
	}

}

void testApp::drawLeftScene(){
    ofPushMatrix();
    
    ofNode n;
    n.setOrientation(oculusRift.getOrientationQuat());
 
    ofQuaternion convergenceQ;
    convergenceQ.makeRotate(converge*.5, n.getUpDir());

    ofMatrix4x4 rm;
    (leftCorrection * convergenceQ).get(rm);
    
    ofMultMatrix(rm);
    drawScene(sphereMeshLeft);
    
    ofPopMatrix();

}

void testApp::drawRightScene(){
    ofPushMatrix();

    ofNode n;
    n.setOrientation(oculusRift.getOrientationQuat());
    
    ofQuaternion convergenceQ;
    convergenceQ.makeRotate(converge*.5, n.getUpDir());

    ofMatrix4x4 rm;
    (rightCorrection * convergenceQ.inverse()).get(rm);
    
    ofMultMatrix(rm);
    drawScene(sphereMeshRight);
    
    ofPopMatrix();
}


//--------------------------------------------------------------
void testApp::drawStringCentered(string str){
	int width = font.stringWidth(str);
	int height = font.stringHeight(str);
	font.drawString(str, ofGetWidth()/2 - width/2, ofGetHeight()/2 - height/2);
}

//--------------------------------------------------------------
void testApp::drawScene(ofMesh& mesh)
{
	ofPushStyle();
	ofPushMatrix();
	
	if(player.isLoaded()){
        player.getTextureReference().bind();
		mesh.draw();
		player.getTextureReference().unbind();
	}
	else{
		videoTestPattern.getTextureReference().bind();
		mesh.draw();
		videoTestPattern.getTextureReference().unbind();
	}
	
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
	
	if(key == 'S'){
		saveSettings();
	}
	
	if(key == 'L'){
		loadSettings();
	}
	
	if(key == 'R'){
		leftCorrection  = ofQuaternion();
		rightCorrection = ofQuaternion();
	}
	
	if(key == ' '){
		if(player.isPlaying()){
			player.stop();
		}
		else{
			player.play();
		}
	}
	
	if(key == OF_KEY_LEFT){
		converge -= .4;
	}
	else if(key == OF_KEY_RIGHT){
		converge += .4;
	}
	
    if(key == OF_KEY_UP){
        showMode = (StereoMode)(int(showMode)+1);
        if(showMode > 2) showMode = (StereoMode)0;
    }
    if(key == OF_KEY_DOWN){
        showMode = (StereoMode)(int(showMode)-1);
        if(showMode < 0) showMode = (StereoMode)2;
    }
//	cout << "X ROTATE " << xRotate << " Y ROTATE " << yRotate << endl;

}

//--------------------------------------------------------------
void testApp::keyReleased(int key)
{

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y)
{
	lastMouse = ofVec2f(x,y);
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button)
{
	
	ofNode n;
	n.setOrientation(oculusRift.getOrientationQuat());

	ofVec2f curMouse(x,y);
	if(x > ofGetWidth()*.1 && x < ofGetWidth()*.9 &&
	   y > ofGetHeight()*.1 && y < ofGetHeight()*.9)
	{
		ofVec2f delta = lastMouse-curMouse;
		ofQuaternion deltaXRot, deltaYRot;
		deltaXRot.makeRotate(delta.x, n.getUpDir());
		deltaYRot.makeRotate(delta.y, n.getSideDir());
		rightCorrection *= (deltaXRot * deltaYRot);
		if( x < ofGetWidth()*.5 ){
			leftCorrection *= (deltaXRot * deltaYRot);
		}
	}
	else{
		float deltaZ = lastMouse.y - y;
		ofQuaternion deltaZRot;
		deltaZRot.makeRotate(deltaZ, n.getLookAtDir());
		rightCorrection *= deltaZRot;
		if( x < ofGetWidth()*.5 ){
			leftCorrection *= deltaZRot;
		}
	}
	
	lastMouse = curMouse;
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
//	if(player.loadMovie( dragInfo.files[0] )){
    if( ofToLower(ofFilePath::getFileExt(dragInfo.files[0])) == "mov" ){
        if(player.loadMovie( dragInfo.files[0] )){
            player.play();
            player.setLoopState(OF_LOOP_NORMAL);
        }
        else{
            ofSystemAlertDialog("Video failed to load.");
        }
    }
    else{
        videoTestPattern.loadImage("test-still.png");
        createMeshWithTexture(videoTestPattern.getTextureReference());
    }
	#endif
	
}
