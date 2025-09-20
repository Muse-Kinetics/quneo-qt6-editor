/****************************************************************/
/*			  KMI QuNeo Mixxx Control Script v1.0				*/
/*					Written with Mixxx v.1.10					*/
/*	  Thanks to Sean M. Pappalardo, Conner Lacy, Matt Hettich	*/
/*						By Diane Douglas						*/
/*							5/25/2012							*/
/****************************************************************/

function KMIQuNeo () {}

//global variables
KMIQuNeo.lastLight = [-1,-1]; // previous LED
KMIQuNeo.trackDuration = [0,0]; // Duration of the song on each deck
KMIQuNeo.recState = 0;
KMIQuNeo.stopState = 0;
KMIQuNeo.playState = 0;
		    
KMIQuNeo.init = function (id) { // called when the device is opened & set up
			engine.connectControl("[Channel1]","visual_playposition","KMIQuNeo.circle1LEDs");
			engine.connectControl("[Channel2]","visual_playposition","KMIQuNeo.circle2LEDs");
		    engine.connectControl("[Master]","crossfader","KMIQuNeo.crossFaderLEDs");
		    engine.connectControl("[Flanger]","lfoDepth","KMIQuNeo.flangerDepth");		    
		    engine.connectControl("[Flanger]","lfoPeriod","KMIQuNeo.flangerPeriod");	
		    engine.connectControl("[Master]","headVolume","KMIQuNeo.headVol");		    
		    engine.connectControl("[Master]","headMix","KMIQuNeo.headMainMix");	
		    engine.connectControl("[Channel1]","volume","KMIQuNeo.player1Vol");		    
		    engine.connectControl("[Channel2]","volume","KMIQuNeo.player2Vol");		    
		    engine.connectControl("[Channel1]","rate","KMIQuNeo.player1Rate");		    
		    engine.connectControl("[Channel2]","rate","KMIQuNeo.player2Rate");	
};

KMIQuNeo.shutdown = function () {
		    
};

KMIQuNeo.rotary1Touch = function (channel, control, value, status, group) {

    if ((status & 0xF0) == 0x90) {    // If note on ch1...
        if (value == 0x7F) {//if note is velocity 127
        var alpha = 1.0/8;
        var beta = alpha/32;
        engine.scratchEnable(1, 128, 33+1/3, alpha, beta);
       		 }
       		 else {engine.scratchDisable(1);}
        }
        
     if (status == 0x80) {    // If button up
        engine.scratchDisable(1);
	}
}

KMIQuNeo.rotary2Touch = function (channel, control, value, status, group) {

    if ((status & 0xF0) == 0x90) {    // If note on ch1...
        if (value == 0x7F) {//if note is velocity 127
        var alpha = 1.0/8;
        var beta = alpha/32;
        engine.scratchEnable(2, 128, 33+1/3, alpha, beta);
       		 }
       		 else {engine.scratchDisable(2);}
        }
        
     if (status == 0x80) {    // If button up
        engine.scratchDisable(2);
	}
}

// The wheel that actually controls the scratching
KMIQuNeo.wheel1Turn = function (channel, control, value, status, group) {
    var newValue;
    if (value > 1) newValue = -1;
    else newValue = 1;
    engine.scratchTick(1,newValue);
}

KMIQuNeo.circle1LEDs = function (value) {

    //LEDs
    //time it takes for an imaginary record to go around once in seconds
	var revtime = 1.8
	//find the current track position in seconds by multiplying total song length
	//by the incoming value of ChannelN visual_playposition 0.0-1.0
	KMIQuNeo.trackDuration[1] = engine.getValue("[Channel1]","duration");
    var currentTrackPos = engine.getValue("[Channel1]","visual_playposition") * KMIQuNeo.trackDuration[1];
    print("current track position1" + currentTrackPos);
    print("track duration" + KMIQuNeo.trackDuration[1]);
    print("play position1" + value);
    //find how many revolutions we have made. The fractional part is where we are on the vinyl.
    var revolutions = (currentTrackPos/revtime) - .25;
    //multiply the fractional part by the total number of LEDs in a rotary.
    var light = ((revolutions-(revolutions|0))*127)|0; 
    //if this is a repeat message, do not send.
    if (KMIQuNeo.lastLight[1]==light) return;
    //format the message CC 6 for rotary 1 CC 7 for rotary 2 on channel 1.
     var byte1 = 0xB0
     var byte2 = 0x06
     var byte3 = 0x00
         midi.sendShortMsg(byte1,byte2,byte3);
	    midi.sendShortMsg(byte1,byte2,byte3+light);
	        KMIQuNeo.lastLight[1]=light;
}

KMIQuNeo.wheel2Turn = function (channel, control, value, status, group) {
    var newValue;
    if (value > 1) newValue = -1;
    else newValue = 1;
    engine.scratchTick(2,newValue);    
}

KMIQuNeo.circle2LEDs = function (value) {

    
    //LEDs
    //time it takes for an imaginary record to go around once in seconds
	var revtime = 1.8
	//find the current track position in seconds by multiplying total song length
	//by the incoming value of ChannelN visual_playposition 0.0-1.0
	KMIQuNeo.trackDuration[2] = engine.getValue("[Channel2]","duration");
    var currentTrackPos = engine.getValue("[Channel2]","visual_playposition") * KMIQuNeo.trackDuration[2];
        print("current track position2" + currentTrackPos);
        print("track duration" + KMIQuNeo.trackDuration[2]);
        print("play position2" + value);
    //find how many revolutions we have made. The fractional part is where we are on the vinyl.
    var revolutions = (currentTrackPos/revtime) - .25;
    //multiply the fractional part by the total number of LED values in a rotary.
    var light = ((revolutions-(revolutions|0))*127)|0; 
    //if this is a repeat message, do not send.
    if (KMIQuNeo.lastLight[2]==light) return;
    //format the message CC 6 for rotary 1 CC 7 for rotary 2 on channel 1.
     var byte1 = 0xB0
     var byte2 = 0x07
     var byte3 = 0x00
         midi.sendShortMsg(byte1,byte2,byte3);
	    midi.sendShortMsg(byte1,byte2,byte3+light);
	        KMIQuNeo.lastLight[2]=light;
    
}

KMIQuNeo.crossFaderLEDs = function (value) {
var crossfadeMidi = (value * 64) + 63;
    midi.sendShortMsg(0xB0,0x05,0x00+crossfadeMidi);
}

KMIQuNeo.flangerDepth = function (value) {
var flangeDepth = value * 127;
    midi.sendShortMsg(0xB0,0x0B,0x00+flangeDepth);
}

KMIQuNeo.flangerPeriod = function (value) {
var flangePeriod = (value - 50) * 127 / 2000000;
    midi.sendShortMsg(0xB0,0x0A,0x00+flangePeriod);
}

KMIQuNeo.headVol = function (value) {
var headLev = value * 127 / 5;
    midi.sendShortMsg(0xB0,0x09,0x00+headLev);
}

KMIQuNeo.headMainMix = function (value) {
var headphoneMainMix = (value + 1) * 127 / 2;
    midi.sendShortMsg(0xB0,0x08,0x00+headphoneMainMix);
}

KMIQuNeo.player1Vol = function (value) {
var play1Vol = value * 127;
    midi.sendShortMsg(0xB0,0x01,0x00+play1Vol);
}

KMIQuNeo.player2Vol = function (value) {
var play2Vol = value * 127;
    midi.sendShortMsg(0xB0,0x02,0x00+play2Vol);
}

KMIQuNeo.player1Rate = function (value) {
var play1Rate = (value + 1) * 127 / 2;
    midi.sendShortMsg(0xB0,0x03,0x00+play1Rate);
}

KMIQuNeo.player2Rate = function (value) {
var play2Rate = (value + 1) * 127 / 2;
    midi.sendShortMsg(0xB0,0x04,0x00+play2Rate);
}

KMIQuNeo.rec = function (channel, control, value, status, group) {
if (KMIQuNeo.recState == 0){
	KMIQuNeo.recState = 1;
    midi.sendShortMsg(0x90,0x21,0x7F);
	}
else {
	KMIQuNeo.recState = 0;
	midi.sendShortMsg(0x90,0x21,0x00);
	}
}
KMIQuNeo.stop = function (channel, control, value, status, group) {
if (KMIQuNeo.stopState == 0){
	KMIQuNeo.stopState = 1;
    midi.sendShortMsg(0x90,0x22,0x7F);
	}
else {
	KMIQuNeo.stopState = 0;
	midi.sendShortMsg(0x90,0x22,0x00);
	}
}
KMIQuNeo.play = function (channel, control, value, status, group) {
if (KMIQuNeo.playState == 0){
	KMIQuNeo.playState = 1;
    midi.sendShortMsg(0x90,0x23,0x7F);
	}
else {
	KMIQuNeo.playState = 0;
	midi.sendShortMsg(0x90,0x23,0x00);
	}
}

KMIQuNeo.rhombus = function (channel, control, value, status, group) {
if (value > 0){midi.sendShortMsg(0x90,0x2E,0x7F);}
else {midi.sendShortMsg(0x90,0x2E,0x00);}
}