#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){

//--------------------------------------------------------------
//Declaring all variables, initializing all textures, setting up video capture.
//Takes a while, eh?

    prefsFile.loadFile("params.xml");       //Get the parameters for image generation

    ofSetLogLevel(OF_LOG_VERBOSE);

    ofBackground(255,255,255);
    ofSetFrameRate(prefsFile.getValue("gl:glmaxframerate",30,0));
    if(prefsFile.getValue("gl:glusesmoothing",1,0) == 1){
        ofEnableSmoothing();
    }
    if(prefsFile.getValue("gl:glusearbtex",1,0) == 1){
        ofEnableArbTex();
    }
    ofEnableAlphaBlending();

    isPlayingIntro = true;

    showInstructions = false;       //Should we show the instructions pane?


    lRotate = 0;

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
	text1.allocate(800,800,OF_IMAGE_COLOR_ALPHA);
	text1.loadImage("img/text1.png");
	text2.allocate(912, 64, OF_IMAGE_COLOR_ALPHA);
	text2.loadImage("text2.png");
	gfxTitle.allocate(817, 458, OF_IMAGE_COLOR_ALPHA);
	gfxTitle.loadImage("img/title.png");

	debugFont.loadFont("arial.ttf", 8);
	introText.loadFont("arial.ttf", 24);
	//introMusic.loadSound("worksong.wav", true);
	introVid.loadMovie("zwcleantitle.mov");
	//vidText1.loadMovie("clip1.mov");

	p1Complete = false;
	p2Complete = false;
	p3Complete = false;
	p4Complete = false;

	currentPattern = 0;

    sDensity = 10;
	//Take care of this crap up front.

    int to = ofRandom(-10, 10);
    /*
    int td = sDensity;
    td += to;
    sDensity = ofClamp(td, 2, 50);
    */

    //Below is the initial load and setting of all relevant variables and reading from XML.
    //There are probably some things in this For loop that could be cut out of it...
    //As one can see, there is much optimization to be done.

    currentPattern = ofRandom(0,2);
    float wh;
    for(int i = 0; i < sDensity; i++){
        sNumShadows[i] = ofRandom(prefsFile.getValue("params:snshadowsmin", 0), prefsFile.getValue("params:snshadowsmax", 0));
        int a = prefsFile.getValue("params:stranslaterange", 0) / 2;
        sTranslateX[i] = ofRandom(a *= -1, a);
        sTranslateY[i] = ofRandom(a *= -1, a);
        int b = prefsFile.getValue("params:sdrawrange", 0);
        sDrawX[i] = ofRandom(b *= -1, b);
        sDrawY[i] = ofRandom(b *= -1, b);
        wh = ofRandom(prefsFile.getValue("params:ssizemodmin",0), prefsFile.getValue("params:ssizemodmax",0));
        sDrawWidth[i] = 640 * wh;
        sDrawHeight[i] = 480 * wh;
        sRotateAngle[i] = 360 / sNumShadows[i];
        sRandomFactor[i] = ofRandom(-100, 100);
        sThetaXMod[i] = ofRandom(prefsFile.getValue("params:sthetamodxmin", 0), prefsFile.getValue("params:sthetamodxmax",0));
        sThetaYMod[i] = ofRandom(prefsFile.getValue("params:sthetaymodmin",0), prefsFile.getValue("params:sthetaymodmax",0));
        mutAngle = ofRandom(prefsFile.getValue("params:mutanglemin",0),prefsFile.getValue("params:mutanglemax",0));
        innerRadius = ofRandom(prefsFile.getValue("params:innerradiusmin",0),prefsFile.getValue("params:innerradiusmax",0));
        outerRadius = ofRandom(prefsFile.getValue("params:outerradiusmin",0),prefsFile.getValue("params:outerradiusmax",0));
        nArms = ofRandom(prefsFile.getValue("params:snarmsmin",0),prefsFile.getValue("params:snarmsmax",0));
        nShadowsPerArm = ofRandom(prefsFile.getValue("params:snshadowsperarmmin",0),prefsFile.getValue("params:snshadowsperarmmax",0));
        nTunnelArms = ofRandom(5,15);
        nShadowsPerTunnelArm = ofRandom(20,30);
        nStars = ofRandom(5, 10);
        nRings = ofRandom(1, 10);
        nRingOffset = ofRandom(500, 700);
        sSeedColor = ofRandom(0,3);
        sSeedRGB = ofRandom(0, 255);
        sAlpha[i] = ofRandom(10, 200);
        if(sSeedColor == 0){
            sRed[i] = ofRandom(200, 255);
            sGreen[i] = ofRandom(0, 100);
            sBlue[i] = ofRandom(0, 100);
        }if(sSeedColor == 1){
            sRed[i] = ofRandom(0, 100);
            sGreen[i] = ofRandom(200, 255);
            sBlue[i] = ofRandom(0,100);
        }if(sSeedColor == 2){
            sRed[i] = ofRandom(0, 100);
            sGreen[i] = ofRandom(0, 100);
            sBlue[i] = ofRandom(200, 255);
        }
    }
    //::::::::::::::::::::::::::::::::::::::::

	nShadows = 50;
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
	if(prefsFile.getValue("init:usesongplayer",1,0) == 1){
        useSongPlayer = true;
	} else {
	    useSongPlayer = false;
	}
	showCamInput = false;
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

    if(useSongPlayer){
        songPlayer.update();
    }

	int introPhase = 1;

	fCount++;

	if(bIntro == true && sMusic == true){       //A bit of code to kick off the little 'ZW' intro vid
		//introMusic.play();
		introMusic.setSpeed(1);
		sMusic = false;
		introVid.play();
		introVid.setLoopState(OF_LOOP_NONE);
	}

	if(introVid.getIsMovieDone() == true){      //Stop the intro vid when it's over and clear the memory used by it
		introVid.stop();
		introVid.close();
	}

	if(ofGetElapsedTimef() >= 11.848){          //Time based triggers for various stages of the lead-in process.
		introPhase = 2;
	}if(ofGetElapsedTimef() >= 13){
		introPhase = 3;
	}
    /*
	if(introMusic.getIsPlaying() == false){
		//introFractal = false;
		introMusic.unloadSound();
	}
	*/

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	if(introPhase == 2 && p2Complete == false){
		//vidText1.setLoopState(OF_LOOP_NONE);
		//vidText1.play();
		p2Complete = true;
	}if(introPhase == 3 && p3Complete == false){    //When finished, start displaying the kaleidoscope, referred to here as an 'introFractal.'
		introFractal = true;
		p3Complete = true;
	}
	//				//
	introVid.idleMovie();
	//vidText1.idleMovie();
	//				//
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
        //Here's the math part.
        //
        //If the boolean value introFractal is true, then the intro process is done and we
        //can start displaying the pretty patterns now. In this part, the theta modifiers
        //are set to random numbers to create a large part of the randomness of the
        //images and to offset them by randem pixel amounts.
        //

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
		if(theta > 360){	theta = 0;	}       //Loop through 'theta' from 0 to 360
		xMouse = cos (ofDegToRad(theta));	xMouse *= ofGetScreenWidth() / 2;       //Not sure why these are called MOUSE values, but they are the basis for
		yMouse = sin (ofDegToRad(theta));	yMouse *= ofGetScreenHeight() / 2;      //the placement of the 'shadows.'
		xMouse *= 2;

		videoGrabber.grabFrame();       //grab a video frame to work on

		colorImg.setFromPixels(videoGrabber.getPixels(), 640, 480);     //Copy the fresh frame into the first of several ofxOpenCV operating image buffers
		colorPreview.loadData(videoGrabber.getPixels(), 640, 480, GL_RGB);      //same here ^^
		grayImg = colorImg;         //Copy one image to another, so they are identical, but one is grayscale. Guess which.
		if(blearnBackground){
			grayBG = grayImg;       //Set the background image into 'grayBG'
			blearnBackground = false;
		}


		grayDiff.absDiff(grayBG, grayImg);      //Get the difference between the background and video frame, this is what separates subject and background.
		grayDiff.threshold(threshold, false);   //Get a black and white mask of the subject with background cut out. This is stored in the difference buffer, 'grayDiff.'

		contourFinder.findContours(grayDiff, 50, 640 * 480, 5, true, true);     //The OpenCV bit, where we find the contours in the difference buffer


#ifdef ZW_ENABLE_RANDOM     //ALWAYS ON
if(contourFinder.nBlobs == 0 || bRefresh == true){      //If there is no detected subject, I.E. no shapes detected by openCV, then refresh the values of shape generation.
    bRefresh = false;                                   //This gives the program it's randomized nature. When no one is in the frame, generate a new pattern
    int to = ofRandom(-10, 10);
    /*
    int td = sDensity;
    td += to;
    sDensity = ofClamp(td, 2, 50);
    */

    //In this next block of code, we read values from XML and randomize those values to give
    //the interesting random aspect to the visualization.
    currentPattern = ofRandom(0,2);
    float wh;
    for(int i = 0; i < sDensity; i++){
        sNumShadows[i] = ofRandom(prefsFile.getValue("params:snshadowsmin", 0), prefsFile.getValue("params:snshadowsmax", 0));
        int a = prefsFile.getValue("params:stranslaterange", 0) / 2;
        sTranslateX[i] = ofRandom(a *= -1, a);
        sTranslateY[i] = ofRandom(a *= -1, a);
        int b = prefsFile.getValue("params:sdrawrange", 0);
        sDrawX[i] = ofRandom(b *= -1, b);
        sDrawY[i] = ofRandom(b *= -1, b);
        wh = ofRandom(prefsFile.getValue("params:ssizemodmin",0), prefsFile.getValue("params:ssizemodmax",0));
        sDrawWidth[i] = 640 * wh;
        sDrawHeight[i] = 480 * wh;
        sRotateAngle[i] = 360 / sNumShadows[i];
        sRandomFactor[i] = ofRandom(-100, 100);
        sThetaXMod[i] = ofRandom(prefsFile.getValue("params:sthetamodxmin", 0), prefsFile.getValue("params:sthetamodxmax",0));
        sThetaYMod[i] = ofRandom(prefsFile.getValue("params:sthetaymodmin",0), prefsFile.getValue("params:sthetaymodmax",0));
        mutAngle = ofRandom(prefsFile.getValue("params:mutanglemin",0),prefsFile.getValue("params:mutanglemax",0));
        innerRadius = ofRandom(prefsFile.getValue("params:innerradiusmin",0),prefsFile.getValue("params:innerradiusmax",0));
        outerRadius = ofRandom(prefsFile.getValue("params:outerradiusmin",0),prefsFile.getValue("params:outerradiusmax",0));
        nArms = ofRandom(prefsFile.getValue("params:snarmsmin",0),prefsFile.getValue("params:snarmsmax",0));
        nShadowsPerArm = ofRandom(prefsFile.getValue("params:snshadowsperarmmin",0),prefsFile.getValue("params:snshadowsperarmmax",0));
        nTunnelArms = ofRandom(5,15);
        nShadowsPerTunnelArm = ofRandom(20,30);
        nStars = ofRandom(5, 10);
        nRings = ofRandom(1, 10);
        nRingOffset = ofRandom(500, 700);
        sSeedColor = ofRandom(0,3);
        sSeedRGB = ofRandom(0, 255);
        sAlpha[i] = ofRandom(10, 200);
        if(sSeedColor == 0){                    //Here, we set the color of a shadow [i] to predominantly red, green, or blue colors.
            sRed[i] = ofRandom(200, 255);       //This is where the randomness in color comes in.
            sGreen[i] = ofRandom(0, 100);
            sBlue[i] = ofRandom(0, 100);
        }if(sSeedColor == 1){
            sRed[i] = ofRandom(0, 100);
            sGreen[i] = ofRandom(200, 255);
            sBlue[i] = ofRandom(0,100);
        }if(sSeedColor == 2){
            sRed[i] = ofRandom(0, 100);
            sGreen[i] = ofRandom(0, 100);
            sBlue[i] = ofRandom(200, 255);
        }
    }
}

#endif

	//TEST: Circular pattern



	//faceRect = contourFinder.blobs[0].boundingRect;

//						*****NOTICE******
//	Changes have been made to ofxCvBlob.h to accommodate our
//	rendering needs. The blobs are not outlines, but filled
//  instead. This change was a simple toggle from ofNoFill()
//  to ofFill() in line 43 of ofxCvBlob.h.
//						*****NOTICE******

}

//--------------------------------------------------------------
void testApp::draw(){




//--------------------------------------------------------------
	int psn = 0;
	ofSetColor(255,255,255,255);
	for(int i = 0; i < contourFinder.nBlobs; i++){  //Draw the initial blobs in a buffer with a basic color and full opacity.
		contourFinder.blobs[i].draw();
	}
	for(int i = 0; i < nShadows; i++){
		shadow[i]->loadScreenData(0,0, 640, 480);   //grab the shapes from the buffer so they can be modified more simply.
	}
	ofSetColor(255,255,255);
	ofRect(0,0,ofGetScreenWidth(), ofGetScreenHeight());    //A white rectangle to hide the buffer ( makes ofBackground() useless )
	int zoomScale = prefsFile.getValue("params:zscale",0);  //Read the zoom-scale param from params.xml

//--------------------------------------------------------------

//And here's the draw code.

    if(currentPattern == ZW_PATTERN_SPIROGRAPH){                        //The Main imagery the system shows; This is the entire algo
        for(int d = 0; d <= sDensity; d++){                             //Density is the number of individual rings of shapes, defined in params.xml
            ofSetColor(sRed[d], sGreen[d], sBlue[d], sAlpha[d]);        //Set the color to be what was determined in the block of randomization code
            for(int i = 0; i < sNumShadows[d]; i++){                    //sNumShadows is the number of images in each ring, so draw each of those
                ofPushMatrix();
                ofTranslate(cX + sTranslateX[d], cY + sTranslateY[d], prefsFile.getValue("params:zscale",0));   //Translate to the appropriate offset and for the given Z-scale
                ofRotateZ(sRotateAngle[d] * i);                                                                 //sRotateAngle is the number of shapes divided by 360, for each step in the rotation
                ofTranslate(xMouse * sThetaXMod[d], yMouse * sThetaYMod[d],0);                                  //Translate again
                shadow[i]->draw(0,0,sDrawWidth[d],sDrawHeight[d]);
                ofPopMatrix();
            }
        }

        //End of main pattern code

        //Start alternate pattern code:
        //I forget exactly how it works...

    }if(currentPattern == ZW_PATTERN_SUPERNOVA){
        bool drawArms = true;
        for(int s = 0; s < nRings; s++){
        for(int q = 0; q < nStars; q++){
        for(int d = 0; d < 1; d++){
            ofSetColor(sRed[d], sGreen[d], sBlue[d], ofClamp(sAlpha[d], 80, 200));
            for(int i = 0; i < sNumShadows[d]; i++){
                ofPushMatrix();
                ofTranslate(ofGetScreenWidth() / 2,nRingOffset,0);
                ofRotateZ(((360 / nStars) * q) + theta);
                ofTranslate(cX, cY + (200 * s), prefsFile.getValue("params:zscale",0));
                ofRotateZ((sRotateAngle[d] * i) + theta);
                ofTranslate(sThetaXMod[d] + 500,sThetaYMod[d] + 500,0);
                innerRadius = sThetaYMod[d];
                shadow[i]->draw(0,0,ofClamp(sDrawWidth[5], 32, 240),ofClamp(sDrawHeight[5],24,120));
                ofPopMatrix();
            }
        }for(int d = 0; d < nArms; d++){
            for(int i = 0; i < nShadowsPerArm; i++){
                if(drawArms == true){
                    ofPushMatrix();
                    ofTranslate(ofGetScreenWidth() / 2,nRingOffset,0);
                    ofRotateZ((360 / nStars) * q);
                    ofTranslate(cX, cY, prefsFile.getValue("params:zscale",0));
                    ofRotateZ(((360 / nArms) * d) + theta);
                    ofTranslate(0, ofClamp(sDrawHeight[5],24,120) * i,0);
                    shadow[i]->draw(0,0,ofClamp(sDrawWidth[5], 32, 240),ofClamp(sDrawHeight[5],24,120) * ((i + 1) * 0.25));
                    ofPopMatrix();
                    drawArms = false;
                }
            }
        }
    }
    }
    }
        //This bit doesn't work. IGNORE.
    if(currentPattern == ZW_PATTERN_TUNNEL){
        for(int i = 0; i < nTunnelArms; i++){
            ofPushMatrix();
            ofRotate((360 / nTunnelArms) * i);
            for(int p = 0; p < nShadowsPerTunnelArm; p++){
                ofTranslate(-215,240 / (p + 1) * p,zoomScale);
                ofSetColor(sRed[p], sGreen[p], sBlue[p]);
                shadow[i]->draw(0,0,320 / (p + 1),240 / (p + 1));
                ofPopMatrix();
            }
        }
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
    if( useSongPlayer ){
        songPlayer.draw(); //Display the song track popup, if applicable
    }

    if( showCamInput ){
        ofSetColor(255,255,255,255);
        videoGrabber.draw(0,ofGetScreenHeight() - 240,320,240); //Display the live feed, if applicable
    }

    if( showInstructions == true ){
        text1.draw((ofGetScreenWidth() / 2) - 300, (ofGetScreenHeight() / 2) - 300,600,600); //Display instructions
    }
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
	}if(key == 49){
	    currentPattern = 0;
	}if(key == 50){
	    currentPattern = 1;
	}if(key == 51){
	    currentPattern = 2;
	}if(key == 'k'){
	    lRotate++;
	}if(key == 'l'){
	    lRotate--;
	}if(key == OF_KEY_RETURN){
	    bRefresh = true;
	}if(key == OF_KEY_RIGHT){
	    songPlayer.skipSong();
	}if(key == OF_KEY_LEFT){
	    songPlayer.lastSong();
	}if(key == OF_KEY_F5){
	    int nc = prefsFile.getValue("params:currentcaps",0);
	    char nd[255] = " ";
	    sprintf(nd, "img/captures/capture%d.png", nc);
	    ofSaveScreen(nd);
	    nc++;
	    prefsFile.setValue("params:currentcaps",nc,0);
	    prefsFile.saveFile("params.xml");
	}
	if(key == OF_KEY_F1){
        if( showInstructions ){
            showInstructions = false;
        }else{
            showInstructions = true;
        }
	}


	if(key == 'p'){
        if( showCamInput ){
            showCamInput = false;
        }else{
            showCamInput = true;
        }
	}if(key == 'o'){
        prefsFile.clear();
        prefsFile.loadFile("params.xml");
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

