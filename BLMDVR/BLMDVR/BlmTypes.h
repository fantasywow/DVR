#pragma once


typedef struct _encodeSetting{
	int audio;
	int quality;
	int format;
	int frameRate;
	int maxBit;
	int isSub;
	int quality_sub;
	int format_sub;
	int frameRate_sub;
	int maxBit_sub;
}BlmEncodeSetting;

typedef struct _userSetting{
	BlmEncodeSetting m_encodeSetting;
}BlmUserSetting;
