#pragma once


typedef struct _encodeSetting{
	int audio;
	int quality;
	int format;
	int frameRate;
	int maxBit;
	int sub;
	int quality_sub;
	int format_sub;
	int frameRate_sub;
	int maxBit_sub;
	_encodeSetting(){
		audio = 0;
		quality=0;
		format=0;
		frameRate=0;
		maxBit=0;
		sub=0;
		quality_sub=0;
		format_sub=0;
		frameRate_sub=0;
		maxBit_sub=0;
	}
}BlmEncodeSetting;
