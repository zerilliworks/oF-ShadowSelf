#ifndef _TEST_APP
#define _TEST_APP

/*ZERILLIWORKS CUSTOMIZATIONS:
	Code has been added to ofxCvBlob.h.
		-Removed some items to make blobs draw as solid shapes.
	Code has been changed in ofGraphics.cpp.
		-Altered the ofBackground function to enable global transparency.
 */

#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxCvBlob.h"
#include "ofxXmlSettings.h"

#include "..\..\..\apps\projects\shadow_cv\zwMusicIndicator.h"

//#define _ZW_DEBUG
#define ZW_ENABLE_RANDOM
#define ZW_PLACFORM_WIN_X86_64
//#define ZW_PLATFORM_WIN_x86
//#define ZW_PLATFORM_OSX_X86_64

const int ZW_PATTERN_SPIROGRAPH = 0;
const int ZW_PATTERN_SUPERNOVA = 1;
const int ZW_PATTERN_TUNNEL = 2;

class testApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed  (int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);

    ofxXmlSettings      prefsFile;

    zwMusicIndicator    songPlayer;

	ofVideoGrabber      videoGrabber;

	ofTexture           colorPreview;
	ofTexture           threshPreview;

	ofTexture**         shadow;

	ofxCvContourFinder  contourFinder;

	ofxCvColorImage     colorImg;

	ofxCvGrayscaleImage grayImg;
	ofxCvGrayscaleImage grayBG;
	ofxCvGrayscaleImage grayDiff;

	ofRectangle         faceRect;

	ofSoundPlayer       introMusic;
	ofImage             gfxTitle;
	ofVideoPlayer       introVid;
	ofVideoPlayer       vidText1;
	ofTrueTypeFont      introText;
	ofImage titles;
	ofImage logo;
	ofImage text1;
	ofImage text2;

	//zwGhost** shadow;
	//zwLiveGhost liveGhost;

	bool bRefresh;
	bool isPlayingIntro;
	bool showInstructions;
	int fadeCounter;

	int nShadows;
	int currentPattern;
	//::::::::::::::::::::::::::::::::::::::
	bool bIntro;
	bool bIntro2;
	bool sMusic;
	int sCount;
	int fCount;
	bool introFractal;
	bool p1Complete;
	bool p2Complete;
	bool p3Complete;
	bool p4Complete;
	float zoomScale;
	//::::::::::::::::::::::::::::::::::::::
	int threshold;
	int nPixels;
	bool blearnBackground;
	char pixAlpha[640 * 480];

	//For circle pattern:
	int nPoints;
	int radius;
	float cX;
	float cY;
	float theta;
	int oldBlobs;

	//For Supernova pattern:
	float mutAngle;
	int nArms;
	int nShadowsPerArm;
	float innerRadius;
	float outerRadius;
	int nStars;
	int nRings;
	int nRingOffset;
	int lRotate;

	//For Arms Pattern:
	int nTunnelArms;
	int nShadowsPerTunnelArm;

	//For randomization:
	float randomVar[9];
    bool allowRandom;
	float thetamod1;
	float thetamod2;
	float thetamod3;
	int patterntype;

	//::::::::::::::::::::::::::::::::::::
	float sTranslateX[255];
	float sTranslateY[255];
	float sDrawX[255];
	float sDrawY[255];
	float sDrawWidth[255];
	float sDrawHeight[255];
	float sRotateAngle[255];
	float sRandomFactor[255];
	float sThetaXMod[255];
	float sThetaYMod[255];
	int   sNumShadows[255];
    int   sSeedRGB;
    int   sSeedColor;   //This will equal 0, 1, or 2 for Red, Green, and Blue, respectively. Tells the computer which hue to create palettes for.
    int   sRed[255];
    int   sGreen[255];
    int   sBlue[255];
    int   sAlpha[255];
    int   sDensity;

	//For debug purposes:
	float xMouse;
	float yMouse;

	int nudgeX;
	int nudgeY;

	bool useSongPlayer;
	bool showCamInput;

	ofTrueTypeFont debugFont;


};

#endif
