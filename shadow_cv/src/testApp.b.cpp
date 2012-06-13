#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){

//--------------------------------------------------------------
    ofBackground(255,255,255);
    ofSetFrameRate(30);
    ofEnableAlphaBlending();
    ofEnableSmoothing();
    ofEnableArbTex();


	grayImg.allocate(640, 480);
    grayBG.allocate(640, 480);
    colorImg.allocate(640, 480);
    grayDiff.allocate(640, 480);
	videoGrabber.setVerbose(true);
	videoGrabber.initGrabber(640, 480, true);
	colorPreview.allocate(640, 480, GL_RGB);
	threshPreview.allocate(640, 480, GL_LUMINANCE);

	titles.allocate(772, 63, OF_IMAGE_COLOR_ALPHA);
	titles.loadImage("title.png");
	text2.allocate(912, 64, OF_IMAGE_COLOR_ALPHA);
	text2.loadImage("text2.png");

	debugFont.loadFont("arial.ttf", 8);
	introText.loadFont("arial.ttf", 24);
	introMusic.loadSound("imaginaryordinary.wav", true);
	introVid.loadMovie("zwcleantitle.mov");
	vidText1.loadMovie("clip1.mov");

	p1Complete = false;
	p2Complete = false;
	p3Complete = false;
	p4Complete = false;

	nShadows = 30;
	theta = 230;
	nudgeX = 0;
	nudgeY = 0;

	for(int i = 0; i < 9; i++){
		randomVar[i] = 1;
	}

	thetamod1 = ofRandom(0,360);
	thetamod2 = ofRandom(0,360);
	thetamod3 = ofRandom(0,360);


//--------------------------------------------------------------

	bIntro = true;
	bIntro2 = false;
	sMusic = true;
	introFractal = false;
	fCount = 0;
	sCount = 0;
	zoomScale = 40000;

	shadow = new ofTexture*[nShadows];

	for(int i = 0; i < nShadows; i++){
		shadow[i] = new ofTexture;
		shadow[i]->allocate(640, 480, GL_RGBA);
	}

	nPoints = 10;
	radius = 260;
	threshold = 70;
	cY = ofGetScreenHeight() / 2;
	cX = ofGetScreenWidth() / 2;

}

//--------------------------------------------------------------
void testApp::update(){

	int introPhase = 1;

	fCount++;

	if(bIntro == true && sMusic == true){
		introMusic.play();
		introMusic.setSpeed(1);
		sMusic = false;
		introVid.play();
		introVid.setLoopState(OF_LOOP_NONE);
	}

	if(introVid.getIsMovieDone() == true){
		introVid.stop();
		introVid.close();
	}

	if(ofGetElapsedTimef() >= 11.848){
		introPhase = 2;
	}if(ofGetElapsedTimef() >= 14.0){
		introPhase = 3;
	}

	if(introMusic.getIsPlaying() == false){
		introFractal = false;
		introMusic.unloadSound();
	}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	if(introPhase == 2 && p2Complete == false){
		vidText1.setLoopState(OF_LOOP_NONE);
		vidText1.play();
		p2Complete = true;
	}if(introPhase == 3 && p3Complete == false){
		introFractal = true;
		p3Complete = true;
	}
	//				//
	introVid.idleMovie();
	//vidText1.idleMovie();
	//				//
//--------------------------------------------------------------

		if(introFractal == true || contourFinder.nBlobs == 0){
			theta+= 0.5;
			zoomScale /= 1.5;
			if(contourFinder.nBlobs == 0){
                thetamod1 = ofRandom(-360,360);
                thetamod2 = ofRandom(-360,360);
                thetamod3 = ofRandom(-360,360);
			}
		}
		oldBlobs = contourFinder.nBlobs;
		if(theta > 360){	theta = 0;	}
		xMouse = cos (ofDegToRad(theta));	xMouse *= ofGetScreenWidth() / 2;
		yMouse = sin (ofDegToRad(theta));	yMouse *= ofGetScreenHeight() / 2;
		xMouse *= 2;

		videoGrabber.grabFrame();

		colorImg.setFromPixels(videoGrabber.getPixels(), 640, 480);
		colorPreview.loadData(videoGrabber.getPixels(), 640, 480, GL_RGB);
		grayImg = colorImg;
		if(blearnBackground){
			grayBG = grayImg;
			blearnBackground = false;
		}


		grayDiff.absDiff(grayBG, grayImg);
		grayDiff.threshold(threshold, false);

		contourFinder.findContours(grayDiff, 10, 640 * 480, 5, true, true);


#ifdef ZW_RANDOMIZE_PATTERN
	if(contourFinder.nBlobs == 0){
		for(int i = 0; i < 9; i++){
			randomVar[i] = ofRandom(-2,2);
		}
	}
#endif

	//TEST: Circular pattern



	//faceRect = contourFinder.blobs[0].boundingRect;

//						*****NOTICE******
//	Changes have been made to ofxCvBlob.h to accommodate our
//	rendering needs.
//						*****NOTICE******

}

//--------------------------------------------------------------
void testApp::draw(){


//--------------------------------------------------------------
	int psn = 0;
	ofSetColor(255,255,255,255);
	for(int i = 0; i < contourFinder.nBlobs; i++){
		contourFinder.blobs[i].draw();
	}
	for(int i = 0; i < nShadows; i++){
		shadow[i]->loadScreenData(0,0, 640, 480);
	}
	ofSetColor(255,255,255);
	ofRect(0,0,ofGetScreenWidth(), ofGetScreenHeight());

//--------------------------------------------------------------
	nPoints = 30;
	int deltaAngle = 360 / nPoints;
	ofSetColor(255, 165, 0, 180);
	for(int i = 0; i < nShadows; i++){
		ofPushMatrix();
		ofTranslate(cX, cY, 0);
		ofRotateZ( 1.5 * (deltaAngle * i) + thetamod3);
		ofTranslate(xMouse + thetamod1 -1100, yMouse + thetamod2 + 40, zoomScale);
		shadow[i]->draw(0,0,32,24);
		ofPopMatrix();
	}
	nPoints = 25;
	deltaAngle = 360 / nPoints;
	ofSetColor(255, 12, 0, 60);
	for(int i = 0; i < nShadows; i++){
		ofPushMatrix();
		ofTranslate(cX, cY, 0);
		ofRotateZ( -5 * (deltaAngle * i) + thetamod3);
		ofTranslate(xMouse + thetamod1 -1500, yMouse + thetamod2 + 40, zoomScale);
		shadow[i]->draw(0,0,180,120);
		ofPopMatrix();
	}
    nPoints = 25;
	deltaAngle = 360 / nPoints;
	ofSetColor(255, 12, 0, 60);
	for(int i = 0; i < nShadows; i++){
		ofPushMatrix();
		ofTranslate(cX, cY, 0);
		ofRotateZ( -5 * (deltaAngle * i) + thetamod3);
		ofTranslate(xMouse + thetamod1 -1300, yMouse + thetamod2 + 100, zoomScale);
		shadow[i]->draw(0,0,180,120);
		ofPopMatrix();
	}
	nPoints = 7;
	deltaAngle = 360 / nPoints;
	ofSetColor(255, 12, 0, 60);
	for(int i = 0; i < nShadows; i++){
		ofPushMatrix();
		ofTranslate(cX, cY, 0);
		ofRotateZ( 1.35 * (deltaAngle * i) + thetamod3);
		ofTranslate(xMouse + thetamod1 -700, yMouse + thetamod2 + 40, zoomScale);
		shadow[i]->draw(0,0,140,80);
		ofPopMatrix();
	}

	nPoints = 7;
	deltaAngle = 360 / nPoints;
	ofSetColor(255, 12, 0, 60);
	for(int i = 0; i < nShadows; i++){
		ofPushMatrix();
		ofTranslate(cX, cY, 0);
		ofRotateZ( 1.35 * (deltaAngle * i) + thetamod3);
		ofTranslate(xMouse + thetamod1 -900, yMouse + thetamod2 + 40, zoomScale);
		shadow[i]->draw(0,0,140,80);
		ofPopMatrix();
	}
	nPoints = 15;
	deltaAngle = 360 / nPoints;
	ofSetColor(255, 0, 42, 100);
	for(int i = 0; i < nShadows; i++){
		ofPushMatrix();
		ofTranslate(cX, cY, 0);
		ofRotateZ((deltaAngle * i) + thetamod3);
		ofTranslate(xMouse + thetamod1, yMouse + thetamod2, zoomScale);
		shadow[i]->draw(0,0,1024,768);
		ofPopMatrix();
	}

	nPoints = 20;
	deltaAngle = 360 / nPoints;
	ofSetColor(0, 127, 255, 30);
	for(int i = 0; i < nShadows; i++){
		ofPushMatrix();
		ofTranslate(cX, cY, 0);
		ofRotateZ( 2 * (deltaAngle * i) + thetamod3);
		ofTranslate((xMouse + thetamod1) / 2, (yMouse + thetamod2) / 2, zoomScale);
		shadow[i]->draw(0,0,420,340);
		ofPopMatrix();
	}

	nPoints = 15;
	deltaAngle = 360 / nPoints;
	ofSetColor(204, 102, 0, 100);
	for(int i = 0; i < nShadows; i++){
		ofPushMatrix();
		ofTranslate(cX, cY, 0);
		ofRotateZ((deltaAngle * i) + thetamod3);
		ofTranslate((xMouse + thetamod1) / 8, (yMouse + thetamod2) / 8, zoomScale);
		shadow[i]->draw(0,0,640,480);
		ofPopMatrix();
	}

	nPoints = 15;
	deltaAngle = 360 / nPoints;
	ofSetColor(51, 204, 153, 80);
	for(int i = 0; i < nShadows; i++){
		ofPushMatrix();
		ofTranslate(cX, cY, 0);
		ofRotateZ( 1.5 * (deltaAngle * i) + thetamod3);
		ofTranslate(xMouse + thetamod1 - 400, yMouse + thetamod2 - 400, zoomScale);
		shadow[i]->draw(0,0,320,240);
		ofPopMatrix();
	}
	nPoints = 4;
	deltaAngle = 360 / nPoints;
	ofSetColor(255, 180, 17, 15);
	for(int i = 0; i < nShadows; i++){
		ofPushMatrix();
		ofTranslate(cX, cY, 0);
		ofRotateZ((deltaAngle * i) + thetamod3);
		ofTranslate(xMouse + thetamod1 - 1200, yMouse + thetamod2, zoomScale);
		shadow[i]->draw(0,0,1280,1024);
		ofPopMatrix();
	}
//--------------------------------------------------------------

	if(bIntro == true && p3Complete == false){
		ofSetColor(255,255,255,255);
		ofRect(0,0, ofGetScreenWidth(), ofGetScreenHeight());
		//vidText1.draw(0,0,ofGetScreenWidth(), ofGetScreenHeight());
		introVid.draw(0,0,ofGetScreenWidth(), ofGetScreenHeight());
	}

//--------------------------------------------------------------

#ifdef _ZW_DEBUG
	colorPreview.draw(704,528,320,240);
	char log[255];
	sprintf(log, "Mouse X: %f   Mouse Y: %f    nX: %d    nY: %d    theta: %f    Seconds: %f", xMouse, yMouse, nudgeX, nudgeY, theta, ofGetElapsedTimef());
	ofSetColor(50, 50, 50);
	debugFont.drawString(log, 0, 20);
#endif
/*
	char log[255];
	sprintf(log, "Theta: %f xM: %f yM: %f", theta,xMouse,yMouse);
	ofSetColor(0, 0, 0, 255);
	debugFont.drawString(log, 0, 10);
	//printf("%d ", ofGetFrameNum());
	*/
}


//--------------------------------------------------------------
void testApp::keyPressed(int key){
	if(key == OF_KEY_UP){
		threshold++;
	}if(key == OF_KEY_DOWN){
		threshold--;
	}if(key == ' '){
		blearnBackground = true;
	}if(key == 'a'){
		nudgeX -= 10;
	}if(key == 'd'){
		nudgeX += 10;
	}if(key == 's'){
		nudgeY -= 10;
	}if(key == 'w'){
		nudgeY += 10;
	}

	if(key == 'p'){
#undef _ZW_DEBUG
	}if(key == 'o'){
#define _ZW_DEBUG
	}
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
#ifdef _ZW_DEBUG
	xMouse = x;
	yMouse = y;
#endif
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

