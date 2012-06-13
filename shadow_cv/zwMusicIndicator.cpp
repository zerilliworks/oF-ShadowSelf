#include "zwMusicIndicator.h"

zwMusicIndicator::zwMusicIndicator(){
    lgSongName.loadFont("arial.ttf", 22);
    smiArtist.loadFont("arial.ttf", 18);
    smAlbum.loadFont("arial.ttf", 18);

    playlist.loadFile("playlist.xml");

    albumCover.allocate(80, 80, OF_IMAGE_COLOR);
    albumCover.loadImage("img/music/blank.png");

    playNext = false;
    showInfo = true;
    goToNextSong = false;

    xmlOffset = 1;
    frames = 0;
    w = 480;
    h = 100;
    x = ofGetScreenWidth() - w;
    y = ofGetScreenHeight();
    tw = 0;
    th = 0;
    tx = 0;
    ty = 0;

    indicatorBg.allocate(w,h,OF_IMAGE_COLOR_ALPHA);
    indicatorBg.loadImage("img/indbg.png");

    char dir[255];
    sprintf(dir, "PLAYLIST:SONG%d:ID", xmlOffset);
    currentSong = playlist.getValue(dir,"");
    currentPlaylist = playlist.getValue("PLAYLIST:NAME","");
    sprintf(dir, "PLAYLIST:SONG%d:ID", xmlOffset + 1);
    nextSong = playlist.getValue(dir, "");
    sprintf(dir, "PLAYLIST:SONG%d:NAME", xmlOffset);
    fullName = playlist.getValue(dir, "");
    sprintf(dir, "PLAYLIST:SONG%d:ALBUM", xmlOffset);
    fullAlbum = playlist.getValue(dir, "");
    sprintf(dir, "PLAYLIST:SONG%d:ARTIST", xmlOffset);
    fullArtist = playlist.getValue(dir, "");
    sprintf(dir, "PLAYLIST:SONG%d:FILE", xmlOffset);
    soundFile = playlist.getValue(dir, "");
}

void zwMusicIndicator::update(){
    if(showInfo == true){
        frames++;
    }
    if(song.getIsPlaying() == false || goToNextSong == true){
        if(song.getIsPlaying() == true){
            song.stop();
            song.unloadSound();
        }
        char dir[255];
        sprintf(dir, "PLAYLIST:SONG%d:ID", xmlOffset);
        currentSong = playlist.getValue(dir,"");
        currentPlaylist = playlist.getValue("PLAYLIST:NAME","");
        sprintf(dir, "PLAYLIST:SONG%d:ID", xmlOffset + 1);
        nextSong = playlist.getValue(dir, "");
        sprintf(dir, "PLAYLIST:SONG%d:NAME", xmlOffset);
        fullName = playlist.getValue(dir, "");
        sprintf(dir, "PLAYLIST:SONG%d:ALBUM", xmlOffset);
        fullAlbum = playlist.getValue(dir, "");
        sprintf(dir, "PLAYLIST:SONG%d:ARTIST", xmlOffset);
        fullArtist = playlist.getValue(dir, "");
        sprintf(dir, "PLAYLIST:SONG%d:FILE", xmlOffset);
        soundFile = playlist.getValue(dir, "");

        int nSongs = playlist.getValue("PLAYLIST:NSONGS",0);

        printf(dir);
        albumCover.clear();
        sprintf(dir, "PLAYLIST:SONG%d:COVER", xmlOffset);
        albumCover.loadImage(playlist.getValue(dir,""));

        song.loadSound(soundFile, true);
        showInfo = true;
        goToNextSong = false;
        tw = 0;
        th = 0;
        tx = 0;
        ty = 0;
        frames = 0;
        song.play();
        if(xmlOffset < nSongs + 1){
        xmlOffset++;
        }else{
            xmlOffset = 0;
        }
    }
    string dir;
}

void zwMusicIndicator::draw(){
    if(showInfo == true){
if(frames < (h / 2)){
    ty+= 2;
}if(frames > (h / 2) && frames < 300){
    //don't do nothin', jhonny.
}if(frames > 300 && frames < 300 + (h / 2)){
    ty-= 2;
}if(frames > 300 + (h / 2)){
    showInfo = false;
}
    }
    char dir[255];

    ofSetColor(0,0,0,180);
    ofRect(x,y - ty,w,h);
    ofSetColor(255,255,255,255);
    albumCover.draw(x + 10, y + 10 - ty);
    sprintf(dir, "PLAYLIST:SONG%d:NAME", xmlOffset - 1);
    lgSongName.drawString(playlist.getValue(dir, ""), x + 100, y - ty + lgSongName.stringHeight(playlist.getValue(dir, "")));
    sprintf(dir, "PLAYLIST:SONG%d:ARTIST", xmlOffset - 1);
    smiArtist.drawString(playlist.getValue(dir, ""), x + 100, y - ty + 35 + lgSongName.stringHeight(playlist.getValue(dir, "")));
    sprintf(dir, "PLAYLIST:SONG%d:ALBUM", xmlOffset - 1);
    smAlbum.drawString(playlist.getValue(dir, ""), x + 100, y - ty + 60 + smiArtist.stringHeight(playlist.getValue(dir, "")));


}

void zwMusicIndicator::skipSong(){
    goToNextSong = true;
}

void zwMusicIndicator::lastSong(){
    goToNextSong = true;
    xmlOffset-= 2;
}


zwMusicIndicator::~zwMusicIndicator(){

}
