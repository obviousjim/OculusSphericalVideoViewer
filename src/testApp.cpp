#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup()
{
	ofBackground(0);
	ofSetLogLevel( OF_LOG_VERBOSE );
	ofSetVerticalSync(true);

	showOverlay = false;
	predictive = true;
	
	ofHideCursor();
	
	oculusRift.baseCamera = &cam;
	oculusRift.setup();
	
	//enable mouse;
	cam.begin();
	cam.end();
	cam.setPosition(0,0,0);

	//ofDisableArbTex();
	ofSpherePrimitive p = ofSpherePrimitive(100.0,100);
	sphereMesh = p.getMesh();
	for(int i = 0; i < sphereMesh.getNumVertices(); i++){
		sphereMesh.setTexCoord(i,sphereMesh.getTexCoord(i) * ofVec2f(1920.0,1080.0));
	}

	videoTexture.allocate(1920, 1080, GL_RGB);

	player.loadMovie("monolake.mov");
	player.play();
}


//--------------------------------------------------------------
void testApp::update()
{
	player.update();
	if(player.isFrameNew()){
		videoTexture.loadData(player.getPixelsRef());
	}
}

//--------------------------------------------------------------
void testApp::draw()
{

	
	if(oculusRift.isSetup()){
		
		//if(showOverlay){
		//	
		//	oculusRift.beginOverlay(-230, 320,240);
		//	ofRectangle overlayRect = oculusRift.getOverlayRectangle();
		//	
		//	ofPushStyle();
		//	ofEnableAlphaBlending();
		//	ofFill();
		//	ofSetColor(255, 40, 10, 200);
		//	
		//	ofRect(overlayRect);
		//	
		//	ofSetColor(255,255);
		//	ofFill();
		//	ofDrawBitmapString("ofxOculusRift by\nAndreas Muller\nJames George\nJason Walters\nElie Zananiri\nFPS:"+ofToString(ofGetFrameRate())+"\nPredictive Tracking " + (oculusRift.getUsePredictiveOrientation() ? "YES" : "NO"), 40, 40);
  //          
  //          ofSetColor(0, 255, 0);
  //          ofNoFill();
  //          ofCircle(overlayRect.getCenter(), 20);
		//	
		//	ofPopStyle();
		//	oculusRift.endOverlay();
		//}
  //      

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
		cam.begin();
		drawScene();
		cam.end();
	}

	//image.draw(0,0);
	
}

//--------------------------------------------------------------
void testApp::drawScene()
{
	ofPushStyle();
	//ofNoFill();
	//ofSetColor(ofColor::red);
	ofScale(1,-1,1);
	
	videoTexture.bind();
	//ofDrawSphere(0,0,0, 100);
	sphereMesh.draw();
	videoTexture.unbind();
	//sphereMesh.drawWireframe();

	ofPopStyle();
    
}

//--------------------------------------------------------------
void testApp::keyPressed(int key)
{
	if( key == 'f' )
	{
		//gotta toggle full screen for it to be right
		ofToggleFullscreen();
	}
	
	if(key == 's'){
		oculusRift.reloadShader();
	}
	
	if(key == 'l'){
		oculusRift.lockView = !oculusRift.lockView;
	}
	
	if(key == 'o'){
		showOverlay = !showOverlay;
	}
	if(key == 'r'){
		oculusRift.reset();
		
	}
	if(key == 'h'){
		ofHideCursor();
	}
	if(key == 'H'){
		ofShowCursor();
	}
	
	if(key == 'p'){
		predictive = !predictive;
		oculusRift.setUsePredictedOrientation(predictive);
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

}
