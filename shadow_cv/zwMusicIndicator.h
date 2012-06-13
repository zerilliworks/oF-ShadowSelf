#ifndef ZWMUSICINDICATOR_H_INCLUDED
#define ZWMUSICINDICATOR_H_INCLUDED

#include "ofMain.h"
#include "ofxXmlSettings.h"
#include "ofImage.h"
#include "ofTrueTypeFont.h"
#include "ofSoundPlayer.h"

class zwMusicIndicator{
    public:

    ofSoundPlayer song;
    ofxXmlSettings playlist;
    ofImage albumCover;
    ofImage indicatorBg;
    ofTrueTypeFont lgSongName;
    ofTrueTypeFont smiArtist;
    ofTrueTypeFont smAlbum;

    zwMusicIndicator();
    ~zwMusicIndicator();

    void update();
    void draw();
    void setSong(string _songname);
    void setPlaylist(string _playlist);
    string * getNextSong();
    void skipSong();
    void lastSong();

    int frames;
    int x;
    int y;
    int w;
    int h;
    int th;
    int tw;
    int tx;
    int ty;

    int xmlOffset;

    bool playNext;
    bool showInfo;
    bool goToNextSong;

    string currentSong;
    string fullName;
    string fullAlbum;
    string fullArtist;
    string currentPlaylist;
    string nextSong;
    string soundFile;
    string coverImageFile;


};

#endif // ZWMUSICINDICATOR_H_INCLUDED
