#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup()
{
	ofBackground(0);
	ofSetLogLevel( OF_LOG_VERBOSE );
	ofSetVerticalSync(true);

	showOverlay = false;
	predictive = true;
	
//	ofHideCursor();
	
	oculusRift.baseCamera = &cam;
	oculusRift.setup();
	
	//enable mouse;
	cam.begin();
	cam.end();
	cam.setPosition(0,0,0);


	#ifndef TARGET_WIN32
	player.setUseTexture(false);
	#endif
	//put video path here:
	
}


//--------------------------------------------------------------
void testApp::update()
{
	player.update();
	if(player.isFrameNew()){
		
		if(!videoTexture.isAllocated() ||
		   videoTexture.getWidth() != player.getWidth() ||
		   videoTexture.getHeight() !=player.getHeight())
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
    }
	else{
		cam.begin();
		drawScene();
		cam.end();
	}

//	videoTexture.draw(0,0);
	
}

//--------------------------------------------------------------
void testApp::drawScene()
{
	ofPushStyle();
	ofScale(1,-1,1);
	
	videoTexture.bind();
	sphereMesh.draw();
	videoTexture.unbind();
	
	ofDisableDepthTest();
	sphereMesh.drawWireframe();
	ofEnableDepthTest();
	
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
	player.loadMovie( dragInfo.files[0] );
	player.play();

}
