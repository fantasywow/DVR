/*!
**	********************************************************************************
**                                     dhvecsystem
**                          Digital Video Recoder xp
**
**   (c) Copyright 1992-2004, ZheJiang Dahua Technology Stock CO.LTD.
**                            All Rights Reserved
**
**	File		: dhvecsystem.h
**	Description	: 
**	Modify		: 2006-1-12		zhouwei		Create the file
**	********************************************************************************
*/

#ifndef __DHSYSSDK_H__ 
#define __DHSYSSDK_H__ 

// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the SYSSDK_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// SYSSDK_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.

#define SYSSDK_API extern "C"__declspec(dllexport)

#define BOARD_MAX 8
#define DSPs_PERBOARD_MAX 4
#define DSP_MAX BOARD_MAX * DSPs_PERBOARD_MAX
#define CHs_PERDSP_MAX 16
#define CHANNEL_MAX DSP_MAX * CHs_PERDSP_MAX 
#define MAX_MASK_RECT 32
#define PIXEL_LEN_420 3 / 2
#define PIXEL_LEN_422 2
#define PIXEL_LEN_RGB24 3
#define BITMSK(bit)	(int)(1 << (bit))
#define YUV(y,u,v) (COLORREF)( (BYTE)(v) | ((WORD)((BYTE)(u))<<8) | ((DWORD)((BYTE)(y))<<16) )
#define WM_DEBUGINFO WM_USER + 999

#define DRAWFUN(x)	void (CALLBACK* x)(long nPort, HDC hDc, LONG nUser)

typedef void (* IMAGE_STREAM_CALLBACK) (UINT channelNumber, DWORD dwDataSize, void * context);
typedef int (* STREAM_DIRECT_READ_CALLBACK)(ULONG channelNumber, void * DataBuf,DWORD Length, int FrameType,void *context);
typedef void (*LOGRECORD_CALLBACK)(char *str, void *context);

/*-----------------------------------------------------------------------
	Inconsistent with other manufactures
-----------------------------------------------------------------------*/
typedef enum 
{
	ENC_QCIF_FORMAT = 1,
	ENC_CIF_FORMAT = 2,
	ENC_MCIF_FORMAT = 3,
	ENC_HCIF_FORMAT = 4,
	ENC_2CIF_FORMAT = 5,
	ENC_HD1_FORMAT = 6,
	ENC_4CIF_FORMAT = 7,
}PictureFormat_t;

#define DEFAULT_STANDARD STANDARD_PAL
typedef enum 
{
	STANDARD_AUTO,
	STANDARD_PAL,
	STANDARD_NTSC,
	STANDARD_720P,
	STANDARD_1080I,
	STANDARD_1080P,
	STANDARD_VGA1280x1024,
	STANDARD_VGA1024x768,
	STANDARD_VGA800x600,
	STANDARD_960HP,
	STANDARD_960HN,

	VIDEO_STANDARDS,
} VideoStandard_t;  

typedef enum {
	PktError = 0,
	PktIFrames = 0x0001,
	PktPFrames = 0x0002,
	PktBBPFrames = 0x0004,
	PktAudioFrames = 0x0008,
	PktMotionDetection = 0x00010,
	PktDspStatus = 0x00020,
	PktOrigImage = 0x00040,
	PktSysHeader = 0x00080,
	PktBPFrames = 0x00100,
	PktSFrames = 0x00200,
	PktSubIFrames = 0x00400,
	PktSubPFrames = 0x00800,
	PktSubBBPFrames = 0x01000,
	PktSubSysHeader = 0x02000,
	PktNoMotionDetect = 0x04000,
	PktSubAudioFrames = 0x08000,
    PktIntelligentSearchFrames = 0x10000,

}FrameType_t;

/*-----------------------------------------------------------------------
	Inconsistent with other manufactures
-----------------------------------------------------------------------*/
enum stream_type
{
	STREAM_TYPE_VIDEO = 2,
	STREAM_TYPE_AUDIO = 1,
	STREAM_TYPE_AVSYNC = 3,
};

typedef enum 
{
	DHVEC0404 		= 0x11,
	DHVEC0804		= 0x12,
	DHVEC1604		= 0x13,	
	DHVEC0804L		= 0x18,	
	DHVEC0804LA		= 0x1a,	
	DHVEC0804LC		= 0x1c,	
	DHVEC0804HA		= 0x1d,	
	DHVEC1604LC		= 0x1e,
	DHVEC0804E		= 0x22,
	DHVEC1604E		= 0x23,
	DHVEC8004HD		= 0x24,
	DHVEC8008HB		= 0x25,
	DHVEC8016HB		= 0x26,
	DHVEC8004HB		= 0x27,
	DHVEC8104HD		= 0x28,
	DHVEC8116HF		= 0x29,
	DHVEC8116HB		= 0x2A,
	INVALID_BOARD_TYPE	=0xffffffff,
}BOARD_TYPE_DS;

typedef enum 
{
	brCBR = 0,
	brVBR = 1,
}BitrateControlType_t;

typedef struct tagVersion
{
	ULONG DspVersion, DspBuildNum;					// DSP Ver and BUILD Number	
				
	ULONG DriverVersion, DriverBuildNum;			// Driver Ver and BUILD Number	
				
	ULONG SDKVersion, SDKBuildNum;					// SDK Ver and BUILD Number	
	
}VERSION_INFO, *PVERSION_INFO;

typedef struct tagChannelCapability
{ 
	UCHAR bAudioPreview;							// Audio Preview	
	
	UCHAR bAlarmIO;									// Alarm Signal	
	
	UCHAR bWatchDog;								// Watch Dog	
	
}CHANNEL_CAPABILITY, *PCHANNEL_CAPABILITY; 

typedef struct
{
	BOARD_TYPE_DS type;								// Board Type	
	
	BYTE sn[16];									// Board Serial Number	
	
	UINT dspCount;									// DSP Number in a Board
	
	UINT firstDspIndex;								// First DSP Index in a Board	

	UINT encodeChannelCount;						// Encode Channel Number in a Board	
	
	UINT firstEncodeChannelIndex;					// First Encode Channel Index in a Board
	
	UINT decodeChannelCount;						// Decode Channel Number in a Board       
	
	UINT firstDecodeChannelIndex;					// First Decode Channel Index in a Board
	
	UINT displayChannelCount;						// Video Output Channel Number in a Board
	
	UINT firstDisplayChannelIndex;					// First Video Output Channel Index in a Board
	
	UINT inputStandard	:8;							// supported input video standard PAL/NTSC/1080P/1080I/720P/960H_P/960H_N
	UINT encodeFormat	:8;							// supported max encode picture format D1/CIF/HD
	UINT imageFormat	:4;							// supported image format JPEG/ORG
	UINT countOSD		:4;							// supported max OSD count 2/8
	UINT imgstmFormat	:8;							// supported max original image stream picture format D1/CIF/(1/4HD)
	
	UINT reserved1;
	UINT reserved2;
	UINT reserved3;
}DS_BOARD_DETAIL;

typedef struct
{
	UINT encodeChannelCount;						// Encode Channel Number in a DSP				

	UINT firstEncodeChannelIndex;					// First Encode Channel Number in a DSP		

	UINT decodeChannelCount;						// Decode Channel Number in a DSP				

	UINT firstDecodeChannelIndex;					// First Decode Channel Number in a DSP		

	UINT displayChannelCount;						// Video Output Channel Number in a DSP		

	UINT firstDisplayChannelIndex;					// First Video Output Channel Index in a DSP	

	UINT reserved1;
	UINT reserved2;
	UINT reserved3;
	UINT reserved4;
}DSP_DETAIL;
 
typedef struct tagMotionData{
	PictureFormat_t PicFormat;
	ULONG HorizeBlocks;
	ULONG VerticalBlocks;
	ULONG BlockSize;
}MOTION_DATA_HEADER, *PMOTION_DATA_HEADER;

//user Encry Info
typedef struct _ENCRY_INFO
{
	BYTE EncryPass[8];								// Encryption PassWord

	BYTE EncryInfo[24];							// Encryption Info

}ENCRY_INFO;

/*-----------------------------------------------------------------------
	Device
-----------------------------------------------------------------------*/
/*
function:		Initiate DSP Device
ret value:		the number of successfully initiated board, -1 fail                                                                                
*/
SYSSDK_API int __stdcall InitDSPs(void);

/*
function:		Unload DSP Device																					
ret value:		0 succeed, -1 fail                                                                                
*/
SYSSDK_API int __stdcall DeInitDSPs(void);

/* 
function:		Reset DSP Device	
DspNumber:		in:		the index of DSP
						0 ~ N-1,N is the total number of proper DSP
ret value:		0 succeed, -1 fail                                                                                
cautions:		use carefully only when the DSP does not work properly	
*/																			
SYSSDK_API int __stdcall ResetDSP(int DspNumber);

/*
function:		Get Board Detail Info
boardNum:		in:		the index of board
						0 ~ N-1, N is the total number of proper board
pBoardDetail:	out:	the addr to store the BOARD info, must not be NULL           
ret value:		0 succeed, -1 fail                                                                                
*/																				
SYSSDK_API int __stdcall GetBoardDetail(UINT boardNum, DS_BOARD_DETAIL *pBoardDetail);

/*
function:		Get DSP Detail Info																				
uiExtIndexDSP:	in:		the index of DSP
						0 ~ N-1,N is the total number of proper DSP
pDspDetail:		out:	the addr to store the DSP info, must not be NULL           
ret value:		0 succeed, -1 fail                                                                                
*/
SYSSDK_API int __stdcall GetDspDetail(UINT uiExtIndexDSP, DSP_DETAIL *pDspDetail);

/*
function:		Get SDK Ver Info
VersionInfo:	out:	the addr to store the version info, must not be NULL
ret value:		0 succeed, -1 fail                                                                                
*/
SYSSDK_API ULONG __stdcall GetSDKVersion(PVERSION_INFO VersionInfo);

/*
function:		Get Channel property
hChannel:		in:		channel handle returned by function ChannelOpen()
Capability:		out:	the addr to store the channel capability info, must not be NULL
ret value:		0 succeed, -1 fail                                                                                
*/
SYSSDK_API int __stdcall GetCapability(HANDLE hChannel, CHANNEL_CAPABILITY *Capability); 

/*
function:		Get Board Info
hChannel:		in:		channel handle returned by function ChannelOpen()
BoardType:      out:	the addr to store the boart type info, must not be NULL, refer to the BOARD_TYPE_DS struct
SerialNo:		out:	the addr to store the board Serial Number, must not be NULL
ret value:		0 succeed, -1 fail                                                                                
*/
SYSSDK_API int __stdcall GetBoardInfo(HANDLE hChannel, ULONG *BoardType, UCHAR *SerialNo);

/* 
function:		Get Board Info by encryption method, use this function to protect your software
uiBoardNum:		in:		the index of board
						0 ~ N-1, N is the total number of proper board
puszSerialNo:	in:		the addr storing 128-byte-length data encrypted by the (De)Encryption lib provided by the manufacture
				out:	the addr storing 128-byte-length encrypted Board Serial Number, use the (De)Encryption lib the get the real Serial Number
						must not be NULL
ret value:		0 succeed, -1 fail
remarks:		process as follows:
				8-byte-length random data --> DH_Encode -->128-byte-length data
				--> GetBoardEncSN --> 128-byte-length encrypted board serial num --> DH_Decode
				--> 16-byte-length board serial num                                                                            
*/
SYSSDK_API int __stdcall GetBoardEncSN(UINT uiBoardNum, UCHAR * puszSerialNo);

/*
function:		Get Error Number
hChannel:		in:		channel handle returned by function ChannelOpen()
DspError:		out:	the addr to store the last DSP error code, must not be NULL
SdkError:		out:	the addr to store the last SDK error code, must not be NULL
ret value:		0 succeed, -1 fail                                                                                
*/
SYSSDK_API int __stdcall GetLastErrorNum(HANDLE hChannel, DWORD *DspError, DWORD *SdkError);

/*
function:		Get Board Count
ret value:		Board count if succeeded, -1 if failed                                                                                
*/
SYSSDK_API unsigned int __stdcall GetBoardCount();

/*
function:		Get Encode Channel Number
ret value:		Total Encode Channel count if succeeded, -1 if failed                                                                                
*/
SYSSDK_API unsigned int __stdcall GetEncodeChannelCount();

/*
function:		Open Channel
iChannelNum:	in:		channel index
						0 ~ N-1,N is the total numder of channel in the system
ret value:		the handle of correspond channel if succeeded, -1 if failed
*/
SYSSDK_API HANDLE __stdcall ChannelOpen(int iChannelNum);

/*
function:		Close Channel
hChannel:		in:		channel handle returned by function ChannelOpen()
ret value:		0 succeed, -1 fail                                                                                
*/
SYSSDK_API int __stdcall ChannelClose(HANDLE hChannel);

/*
function:		Get DSP Number
ret value:		DSP count if succeeded, -1 if failed                                                                                
*/
SYSSDK_API int __stdcall GetTotalDSPs();

/*
function:		Get Channel Number
ret value:		total channel count if succeeded, -1 if failed
*/
SYSSDK_API int __stdcall GetTotalChannels();

/*-----------------------------------------------------------------------
	AV
-----------------------------------------------------------------------*/
typedef struct tagFramsStatistics
{
	ULONG VideoFrames;								// Video Frames 

	ULONG AudioFrames;								// Audio Frames

	ULONG FramesLost;		                        // Lost Frames

	ULONG QueueOverflow;	                        // Queue OverFlow

	ULONG CurBps;			                        //Current BitRate(kb/s)

}FRAMES_STATISTICS, *PFRAMES_STATISTICS;

/*
function:		Set Primary Channel Encode Stream Type(Audio Only, Video Only, A&V Mixture)
hChannelHandle:	in:		channel handle returned by function ChannelOpen()
Type:			in:		Enc Stream Type, refer to stream_type enum struct
ret value:		0 succeed, -1 fail                                                                                
*/
SYSSDK_API int __stdcall SetStreamType(HANDLE hChannelHandle, USHORT Type);

/*
function:		Get Primary Channel Encode Stream Type(Audio Only, Video Only, A&V Mixture)
hChannel:		in:		channel handle returned by function ChannelOpen()
StreamType:		out:	addr to store the type info, must not be NULL
ret value:		0 succeed, -1 fail                                                                                
*/
SYSSDK_API int __stdcall GetStreamType(HANDLE hChannel, USHORT *StreamType);

/*
function:		Get Video Signal State, Used to Alarm (Normal, Lost, Blind)
hChannel:		in:		channel handle returned by function ChannelOpen()
ret value:		-1 if failed, else the Video Signal State, refer to the video_state enum state                                                                             
*/
SYSSDK_API int __stdcall GetVideoSignal(HANDLE hChannel);

/*
function:		Set Motion Detection Region
hChannel:		in:		channel handle returned by function ChannelOpen()
rectList:		in:		addr storing the RECT Arry, must not be NULL and the number of RECT can not be less than numberOfAreas
numberOfAreas:	in:		the number of RECT in rectList Arry
ret value:		0 succeed, -1 fail                                                                                
*/
SYSSDK_API int __stdcall SetupMotionDetection(HANDLE hChannel, RECT *rectList, int numberOfAreas);

/*
function:		Get statistical Frame Info
hChannelHandle: in:		channel handle returned by function ChannelOpen()
framesStatistics:out:	addr to store the Frame Rate info, must not be NULL
ret value:		0 succeed, -1 fail  
*/
SYSSDK_API int __stdcall GetFramesStatistics(HANDLE hChannelHandle, PFRAMES_STATISTICS framesStatistics);

/*
function:		Get Original Image Video
hChannel:		channel handle returned by function ChannelOpen()
ImageBuf:		out:	buffer to store the Image, must not be NULL
Size:			in:		tell the length of ImageBuf
				out:	indicate the actual data length in ImageBuf,must not be NULL
ret value:		0 succeed, -1 fail  
*/
SYSSDK_API int __stdcall GetOriginalImage(HANDLE hChannel, UCHAR *ImageBuf, ULONG *Size);

/*
function:		Set Encode Picture Format
hChannel:		in:		channel handle returned by function ChannelOpen()
PictureFormat:	in:		encode picture format, refer to PictureFormat_t enum struct
ret value:		0 succeed, -1 fail  
*/
SYSSDK_API int __stdcall SetEncoderPictureFormat(HANDLE hChannel, PictureFormat_t PictureFormat);

/*
function:		Set Video Standard
hChannel:		in:		channel handle returned by function ChannelOpen()
VideoStandard:	in:		video standard, refer to VideoStandard_t enum struct
ret value:		0 succeed, -1 fail  
*/
SYSSDK_API int __stdcall SetVideoStandard(HANDLE hChannel, VideoStandard_t VideoStandard);

/*
function:		Set Default Video Standard
VideoStandard:	in:		video standard, refer to VideoStandard_t enum struct
ret value:		0 succeed, -1 fail  
cautions:		this function must be called before InitDSPs() 
*/
SYSSDK_API int __stdcall SetDefaultVideoStandard(VideoStandard_t VideoStandard);

/*
function:		Set Video Parameter
hChannelHandle:	in:		channel handle returned by function ChannelOpen()
Brightness:		in:		brightness, 0 ~ 255, default: 115
Contrast:		in:		contrast, 0 ~ 255, default: 115
Saturation:		in:		saturation, 0 ~ 255, default: 125
Hue:			in:		hue, 0 ~ 255, default: 128
ret value:		0 succeed, -1 fail  
*/
SYSSDK_API int __stdcall SetVideoPara(HANDLE hChannelHandle, int Brightness, int Contrast, int Saturation, int Hue);

/*
function:		Get Video Parameter
hChannelHandle:	in:		channel handle returned by function ChannelOpen()
VideoStandard:	out:	addr to store the video standard of correspond channel, must not be NULL
Brightness:		out:	addr to store the video brightness of correspond channel, must not be NULL
Contrast:		out:	addr to store the video contrast of correspond channel, must not be NULL
Saturation:		out:	addr to store the video saturation of correspond channel, must not be NULL
Hue:			out:	addr to store the video hue of correspond channel, must not be NULL
ret value:		0 succeed, -1 fail
*/
SYSSDK_API int __stdcall GetVideoPara(HANDLE hChannelHandle, VideoStandard_t *VideoStandard, int *Brightness, int *Contrast, int *Saturation, int *Hue);

/*
function:		Set Audio Pre Switch (Turn On or Off, Only One Audio Channel Can Be On At The Same Time)
hChannelHandle:	in:		channel handle returned by function ChannelOpen()
bEnable:		in:		switch
						true:  open
						false: close
ret value:		0 succeed, -1 fail
*/
SYSSDK_API int __stdcall SetAudioPreview(HANDLE hChannelHandle, BOOL bEnable);

/*
function:		Read Video Stream Data
DataBuf:		out:	buffer to store data, must not be NULL
Length:			in:		tells the length of buffer DataBuf, must not be NULL
				out:	tells the actual data length in buffer DataBuf
FrameType:		out:	tells the frame type of retreived data in DataBuf, must not be NULL
ret value:		0 succeed, -1 fail
*/
SYSSDK_API int __stdcall ReadStreamData(HANDLE hChannelHandle, void *DataBuf, DWORD *Length, int *FrameType);

/*
function:		Set Bitrate Control Mode (VBR or CBR), used in conjunction with SetupBitrateControl
hChannel:		in:		channel handle returned by function ChannelOpen()
brc:			in:		bitrate control type, refer to BitrateControlType_t enum struct
						brCBR:	constant bitrate mode
						brVBR:	variable bitrate mode(default)
ret value:		0 succeed, -1 fail
*/
SYSSDK_API int __stdcall SetBitrateControlMode(HANDLE hChannel, BitrateControlType_t brc);

/*
function:		Set Max Bitrate,  used in conjunction with SetBitrateControlMode
hChannel:		in:		channel handle returned by function ChannelOpen()
MaxBps:			in:		max bitrate
						0 ~ 32*1024 bps:	do not use MaxBps 
						>= 32*1024 bps:		use MaxBps
						defalult:			5*1024*1024kbps
ret value:		0 succeed, -1 fail
*/	
SYSSDK_API int __stdcall SetupBitrateControl(HANDLE hChannel, ULONG MaxBps);

enum
{
	TYPE_CURRENT_CONFLICT = 1,
};

/*
function:		Start Blind Detection
hChannel:		in:		channel handle returned by function ChannelOpen()
iType:			in:		detection type, refer to blind_type enum struct
iBlackValue:	in:		black detection threshold
						0(darker) ~ 255(brighter),
						default: 32
iWhiteValue:	in:		white detection threshold(currently not use)
						0(darker) ~ 255(brighter),
						default value: 240	
iFrameInterval:	in:		detection frame interval
						default: 25, just like once per second	
iSensitive:		in:		detection sensitive
						0(most sensitive) ~ 255
						default: 32
ret value:		0 succeed, -1 fail
*/
SYSSDK_API int __stdcall StartBlindDetection(HANDLE hChannel, int iType, int iBlackValve, int iWhiteValve, int iFrameInterval, int iSensitive);

/*
function:		Stop Blind Detection 
hChannel:		in:		channel handle returned by function ChannelOpen()
ret value:		0 succeed, -1 fail
*/
SYSSDK_API int __stdcall StopBlindDetection(HANDLE hChannel);

/*
function:		Start Motion Detection	
hChannel:		in:		channel handle returned by function ChannelOpen()
ret value:		0 succeed, -1 fail
*/
SYSSDK_API int __stdcall StartMotionDetection(HANDLE hChannel);

/*
function:		Stop Motion Detection
hChannel:		in:		channel handle returned by function ChannelOpen()
ret value:		0 succeed, -1 fail
*/
SYSSDK_API int __stdcall StopMotionDetection(HANDLE hChannel);

/*
function:		Adjust Motion Detection Precision
hChannel:		in:		channel handle returned by function ChannelOpen()
iGrade:			in:		motion detection sensitivity
						0(most sensitive) ~ 15, different with other manufactures
						normally use 6						 
iFastMotionDetectFps:	in:	fast motion detection fps
							0 ~ 25(slower), 0: do not process fast motion detection
							normally use 5, just about once every quarter second
iSlowMotionDetectFps:	in: slow motion detection fps
							0 ~ 25(slower), 0: do not process slow motion detection
							normally use 25, just about once per second
ret value:		0 succeed, -1 fail
*/
SYSSDK_API int __stdcall AdjustMotionDetectPrecision(HANDLE hChannel, int iGrade, int iFastMotionDetectFps, int iSlowMotionDetectFps);

/*
function:		Set Image Stream Parameter
hChannel:		in:		channel handle returned by function ChannelOpen()
bStart:			in:		swith
						TRUE:	start
						FALSE:	stop
fps:			in:		framerate
						NTSC:	1,2,3,4,5,6,7,10,15,30;
						PAL:	1,2,3,4,5,6,8,12,25;
width:			in:		image width
height:			in:		image height
imageBuffer:	out:	buffer to store the image data
ret value:		0 succeed, -1 fail
*/
SYSSDK_API int __stdcall SetImageStream(HANDLE hChannel,BOOL bStart,UINT fps,UINT width,UINT height,unsigned char *imageBuffer);

/*
function:		Set Image Sample Quotiety
hChannelHandle:	in:		channel handle returned by function ChannelOpen()
IQuantVal:		in:		I frame Quotiety, 12(better quality) ~ 30, default: 16
PQuantVal:		in:		P frame Quotiety, currently not used
BQuantVal:		in:		B frame Quotiety, currently not used
ret value:		0 succeed, -1 fail
*/
SYSSDK_API int __stdcall SetDefaultQuant(HANDLE hChannelHandle, int IQuantVal, int PQuantVal, int BQuantVal);

/*
function:		Set Video Input Position
hChannel:		in:		channel handle returned by function ChannelOpen()
x:				in:		x position, 2 ~ 16, default: 8
y:				in:		y position,	1 ~ 5, default: 1
ret value:		0 succeed, -1 fail
*/
SYSSDK_API int __stdcall SetInputVideoPosition(HANDLE hChannel,UINT x,UINT y);

/*
function:		Set Overlay  Key Color
DestColorKey:	in:		used for overlay display background color, 
						when in overlay mode you must paint the background with this color manualy
ret value:		0 succeed, -1 fail
*/
SYSSDK_API int __stdcall SetOverlayColorKey(COLORREF DestColorKey);

/*
function:		Capture a I Frame, force DSP to encode a I frame
hChannel:		in:		channel handle returned by function ChannelOpen()
ret value:		0 succeed, -1 fail
*/
SYSSDK_API int __stdcall CaptureIFrame(HANDLE hChannel);

/*
function:		Set Encode Frame Rate and key frame interval
hChannel:		in:		channel handle returned by function ChannelOpen()
iKeyFrameIntervals: in:	I frame Interval 1 ~ 255, default: 25(PAL)
iBFrames:		in:		reserved
iPFrames:		in:		reserved
iFrameRate:		in:		frame rate
						NTSC£º1,2,3,4,5,6,7,10,15,30
						PAL£º 1,2,3,4,5,6,8,12,25
ret value:		0 succeed, -1 fail
*/
SYSSDK_API int __stdcall SetIBPMode(HANDLE hChannel, int iKeyFrameIntervals, int iBFrames, int iPFrames, int iFrameRate);

/*
function:		Start Sub Channel Video Capture
hChannel:		in:		channel handle returned by function ChannelOpen()
ret value:		0 succeed, -1 fail
*/
SYSSDK_API int __stdcall StartSubVideoCapture(HANDLE hChannel);

/*
function:		Stop Sub Channel Video Capture
hChannel:		in:		channel handle returned by function ChannelOpen()
ret value:		0 succeed, -1 fail
*/
SYSSDK_API int __stdcall StopSubVideoCapture(HANDLE hChannel);

/*
function:		Set Sub Channel Stream Type
hChannel:		in:		channel handle returned by function ChannelOpen()
Type:			in:		stream type, refer to stream_type enum struct
ret value:		0 succeed, -1 fail
*/
SYSSDK_API int __stdcall SetSubStreamType (HANDLE hChannel, USHORT Type) ;

/*
function:		Get Sub Channel Stream Type
hChannel:		in:		channel handle returned by function ChannelOpen()
Type:			out:	addr to put stream type, must not be NULL, refer to stream_type enum struct
ret value:		0 succeed, -1 fail
*/
SYSSDK_API int __stdcall GetSubStreamType(HANDLE hChannel, USHORT *StreamType);

/*
function:		this function is not currently implemented
ret value:		always returns 0
*/
SYSSDK_API int __stdcall GetSubChannelStreamType(void  *DataBuf, int FrameType);

/*
function:		Set Sub Channle Encode Picture Format
hChannelHandle:	in:		channel handle returned by function ChannelOpen()
PictureFormat:	in:		encode picture format, refer to PictureFormat_t enum struct
ret value:		0 succeed, -1 fail
*/
SYSSDK_API int __stdcall SetSubEncoderPictureFormat(HANDLE hChannelHandle, PictureFormat_t PictureFormat);

/* 
function:		Set Video Detection Precision (this Function is not Currently Implemented)
ret value:		always returns 0
*/
SYSSDK_API int __stdcall SetVideoDetectPrecision (HANDLE hChannel,unsigned int value);

/*
function:		Motion Analyze (used to analyze the motion detection frame data)
hChannelHandle:	in:		channel handle returned by function ChannelOpen()
MotionData:		in:		data send by sdk, must not be NULL
iThreshold:		in:		process motion threshold, used to calculate
						0 ~ 255(intense).
iResult:		out:	process result, 
						the length must not be shorter than the number of RECTs set by SetupMotionDetection, 
						must not be NULL.
						0: no motion
						1: motion
ret value:		0 succeed, -1 fail
*/
SYSSDK_API int __stdcall MotionAnalyzer(HANDLE hChannelHandle, char *MotionData, int iThreshold, int *iResult);

/*
function:		set video matrix out.
nEncodeChannel:	in:		encode channel handle.
nPort:			in:		port number(0 or 1).
bOpen:			in:		0: close
						1: open
nDisplayChannel:in:		display channel index
nDisplayRegion:	in:		region in display channel to output.
nReserved:		in:		reserved, always pass 0.
ret value:		0 succeed, -1 fail
*/
SYSSDK_API int __stdcall SetEncoderVideoExtOutput(UINT nEncodeChannel,UINT nPort,BOOL bOpen,UINT nDisplayChannel,UINT nDisplayRegion,UINT nReserved);
//-----------------------------------------------------------------------

/*-----------------------------------------------------------------------
	OSD
-----------------------------------------------------------------------*/
typedef struct
{
	UINT left;
	UINT top;
	UINT width;
	UINT height;
	COLORREF color;
	UINT param;
}REGION_PARAM;

/*
function:		Update The DSP Timer
hChannel:		in:		channel handle returned by function ChannelOpen()
now:			in:		time struct used to update the encode time in frames
						NULL: use current system time
ret value:		0 succeed, -1 fail
cautions:		this function is different with other manufactures
*/
SYSSDK_API int __stdcall SetupDateTime(HANDLE hChannel, SYSTEMTIME *now);

/*
function:		Set Logo Display Mode
hChannel:		in:		channel handle returned by function ChannelOpen()
ColorKey:		in:		key color used to transparent display
Translucent:	in:		switch
						TRUE:	transparent process
						FALSE:	not transparen process
Twinkle:		in:		twinkle time setting
						0xXXYY,
						XX:	show during time
						YY:	hide during time
						0x0000:	normal display
ret value:		0 succeed, -1 fail
*/
SYSSDK_API int __stdcall SetLogoDisplayMode(HANDLE hChannel, COLORREF ColorKey, BOOL Translucent, int Twinkle);

/*
function:		Set Logo
hChannel:		in:		channel handle returned by function ChannelOpen()
x:				in:		x position, 0 ~ 703
y:				in:		y position, 0 ~ 575
w:				in:		width of logo image, 0 ~ 128, must be 4-pixel-align
h:				in:		height of logo image, 0 ~ 128, must be 4-pixel-align
yuv:			in:		YUV422 format logo image data, length must not be shorter than w*h*2
						must not be NULL
ret value:		0 succeed, -1 fail
*/
SYSSDK_API int __stdcall SetLogo(HANDLE hChannel, int x, int y, int w, int h, unsigned char *yuv); 

/*
function:		Stop Logo
hChannel:		in:		channel handle returned by function ChannelOpen()
ret value:		0 succeed, -1 fail
*/
SYSSDK_API int __stdcall StopLogo(HANDLE hChannel); 

/*
function:		OSD Switch
hChannel:		in:		channel handle returned by function ChannelOpen()
Enable:			in:		switch
						TRUE: turn on
						FALSE:turn off
ret value:		0 succeed, -1 fail
*/
SYSSDK_API int __stdcall SetOsd(HANDLE hChannel, BOOL Enable); 

/*
function:		Set OSD Display Mode
hChannel:		in:		channel handle returned by function ChannelOpen()
Brightness:		in:		osd brightness, 0(dark) ~ 255
Translucent:	in:		transparent mark
						TRUE: transparent display
						FALSE: no transparent display
param:			in:		param
						bit0: auto brightness reverse, 1: YES, 0: NO
						bit16~23: vertical zoom factor, 0: automatic, 1: x1, 2: x2, 4: x4
						bit24~31: horizonal zoom factor, 0: automatic, 1: x1, 2: x2, 4: x4
Format1:		in:		formatted string, must not be NULL
						defines:
						USHORT X£¬ USHORT Y£¬ CHAR0£¬ CHAR1£¬ CHAR2£¬¡­ CHARN
						x: start x potion
						y: start y potion
						CHAR0~CHAR1: may be ASCII, UNICODE or defined MACRO(refer to osd_time_format enum struct)
Format2:		in:		formatted string, must not be NULL
						define same to Format1
ret value:		0 succeed, -1 fail
*/
SYSSDK_API int __stdcall SetOsdDisplayMode(HANDLE hChannel, int Brightness, BOOL Translucent, int param, USHORT *Format1, USHORT *Format2);

/*
function:		Save YUV Data to BMP File
FileName:		in:		file name, must not be NULL
yuv:			in:		YUV422 format data buffer, 
						length must not be short than Width* Height*2
						must not be NULL
Width:			in:		image Width
Height:			in:		image Height
ret value:		0 succeed, -1 fail
*/
SYSSDK_API int __stdcall SaveYUVToBmpFile(char *FileName, unsigned char *yuv, int Width, int Height);

/*
function:		Get YUV Data From BMP File
FileName:		in:		file name, must not be NULL
yuv:			out:	buffer to store the YUV422 data, must not be NULL
BufLen:			in:		tells the length of yuv buffer
Width:			out:	actual image width, must not be NULL
Height:			out:	actual image height, must not be NULL
ret value:		0 succeed, -1 fail
*/SYSSDK_API int __stdcall LoadYUVFromBmpFile(char *FileName, unsigned char *yuv, int BufLen, int *Width, int *Height);

/*
function:		Set Mask Region
hChannelHandle:	in:		channel handle returned by function ChannelOpen()
rectList:		in:		RECT arry, actual number must not be less than iAreas, must not be NULL		
iAreas:			in:		tells the number in rectList arry
ret value:		0 succeed, -1 fail
*/
SYSSDK_API int __stdcall SetupMask(HANDLE hChannelHandle, RECT *rectList, int iAreas);

/*
function:		Stop Mask
hChannelHandle:	in:		channel handle returned by function ChannelOpen()
ret value:		0 succeed, -1 fail
*/
SYSSDK_API int __stdcall StopMask(HANDLE hChannelHandle);
//-----------------------------------------------------------------------
// [2006-3-13]
//-----------------------------------------------------------------------

/*
function:		set display region param
nDisplayChannel: in:	display channel index
nRegionCount:	 in:	total region count(1~16)
pParam:			 in:	region param vector,see definition of REGION_PARAM
nReserved:		 in:	reserved, always pass 0.
ret value:		0 succeed, -1 fail
*/
SYSSDK_API int __stdcall SetDisplayRegion(UINT nDisplayChannel,UINT nRegionCount, REGION_PARAM *pParam,UINT nReserved);

/*
function:		clear display region, note: if this region is displaying, this function has no effect.
nDisplayChannel: in:	display channel index
nRegionFlag:	 in:	region mask, bit0~15 stands for region 0~15 to clear.
						the color used to clear the region is set by SetDisplayRegion's pParam.
ret value:		0 succeed, -1 fail
*/
SYSSDK_API int __stdcall ClearDisplayRegion(UINT nDisplayChannel,UINT nRegionFlag);
//-----------------------------------------------------------------------

/*-----------------------------------------------------------------------
	MasterEngin
-----------------------------------------------------------------------*/

/*
function:		Start Video Preview
hChannelHandle:	in:		channel handle returned by function ChannelOpen()
WndHandle:		in:		window handle used to display
rect:			in:		display region
						NULL: full window
						else: use region
bOverlay:		in:		overlay mode, currently replaced by call SetPreviewOverlayMode
						TRUE: overlay mode
						FALSE: no overlay mode
VideoFormat:	in:		video format
FrameRate:		in:		preview frame rate
						NTSC£º1,2,3,4,5,6,7,10,15,30
						PAL£º 1,2,3,4,5,6,8,12,25¡¡
ret value:		0 succeed, -1 fail
*/
SYSSDK_API int __stdcall StartVideoPreview(HANDLE hChannelHandle,HWND WndHandle, RECT *rect, BOOLEAN bOverlay, int VideoFormat, int FrameRate);

/*
function:		Stop Video Preview
hChannelHandle:	in:		channel handle returned by function ChannelOpen()
ret value:		0 succeed, -1 fail
*/
SYSSDK_API int __stdcall StopVideoPreview(HANDLE hChannelHandle);

/*
function:		Start Video Capture
hChannelHandle:	in:		channel handle returned by function ChannelOpen()
ret value:		0 succeed, -1 fail
*/
SYSSDK_API int __stdcall StartVideoCapture(HANDLE hChannelHandle);

/*
function:		Stop Video Capture
hChannelHandle:	in:		channel handle returned by function ChannelOpen()
ret value:		0 succeed, -1 fail
*/
SYSSDK_API int __stdcall StopVideoCapture(HANDLE hChannelHandle);

/*
function:		Register Callback Draw Function
dwPort:			in:		channel index,
						0 ~ N-1, N: the number of channels in system
DrawFun:		in:		call back draw function, refer to DRAWFUN define
						must not be NULL
lUser:			in:		call back contex
ret value:		0 succeed, -1 fail
*/
SYSSDK_API int __stdcall RegisterDrawFun(DWORD dwPort, DRAWFUN(DrawFun), LONG lUser);

/*
function:		UnRegister Callback Draw Function
dwPort:			in:		channel index,
						0 ~ N-1, N: the number of channels in system
ret value:		0 succeed, -1 fail
*/
SYSSDK_API int __stdcall StopRegisterDrawFun(DWORD nport);

/*
function:		Register Callback Function Used to Get Original Image Data
piscFun:		in:		original image call back function,
						used to read original image stream,
						refer to IMAGE_STREAM_CALLBACK define
context:		in:		call back context
ret value:		0 succeed, -1 fail
*/
SYSSDK_API int __stdcall RegisterImageStreamCallback(IMAGE_STREAM_CALLBACK piscFun, void * context);

/*
function:		Register Callback Function Used to Get Capture Video Data
StreamDirectReadCallback: in:		encode data read call back function,
									used to read encode data,
									refer to STREAM_DIRECT_READ_CALLBACK define
Context:		in:		call back context
ret value:		0 succeed, -1 fail
*/
SYSSDK_API int __stdcall RegisterStreamDirectReadCallback(STREAM_DIRECT_READ_CALLBACK StreamDirectReadCallback, void* Context);

/*
function:		Register a MessageID and a Window Handle Used to Get Notified When the Capture Data Arrived
hWnd:			in:		destination window handle used to receive notification 
MessageId:		in:		message id
ret value:		0 succeed, -1 fail
*/
SYSSDK_API int __stdcall RegisterMessageNotifyHandle(HWND hWnd, UINT MessageId);
//-----------------------------------------------------------------------
// [2006-3-17]
//-----------------------------------------------------------------------

/*
function:		this Function is not Currently Implemented
ret value:		always returns 0
*/
SYSSDK_API int __stdcall RegisterLogRecordCallback(LOGRECORD_CALLBACK LogRecordFunc, void *Context);
//-----------------------------------------------------------------------

//-----------------------------------------------------------------------
/*
function:		this Function is not Currently Implemented
ret value:		always returns 0
*/
SYSSDK_API int __stdcall SetPreviewOverlayMode(BOOL bTrue);

/*
function:		Get Jpeg Image
hChannelHandle:	in:		channel handle returned by function ChannelOpen()
ImageBuf:		out:	buffer to store the jpeg data,
						must not be NULL
Size:			in:		tells ImageBuf length
				out:	indicate actual data length in ImageBuf
nQuality:		in:		image Quality
						1 ~ 100(better Quality)
ret value:		0 succeed, -1 fail
*/
SYSSDK_API int __stdcall GetJpegImage(HANDLE hChannelHandle,UCHAR *ImageBuf,ULONG *Size,UINT nQuality);

/*
function:		Switch between Primary and Sub Channel (Used when Setting the Bitrate Parameter)
hChannelHandle:	in:		channel handle returned by function ChannelOpen()
iSubChannel:	in:		switch
						0(default): switch to primary channel
						1: switch to sub channel
ret value:		0 succeed, -1 fail
*/
SYSSDK_API int __stdcall SetupSubChannel(HANDLE hChannelHandle, int iSubChannel);

/*
function:		Start Intelligent Search (Used to quickly position in Capture File)
hChannelHandle:	in:		channel handle returned by function ChannelOpen()
ret value:		0 succeed, -1 fail
remarks:		when use this function, if system detects motion, you will get a frame: 16 byte-head:PktIntelligentSearchFrames(0x10000),
				with 396(22x18) bytes data followed, each byte(0~255) presents the motion intensity of a 32x32-pixel-block, 
				save it in the capture file and use it to quickly position in file according to the region and intensity.
*/
SYSSDK_API int __stdcall StartIntelligentSearch(HANDLE hChannelHandle);

/*
function:		Stop Intelligent Search
hChannelHandle:	in:		channel handle returned by function ChannelOpen()
ret value:		0 succeed, -1 fail
*/
SYSSDK_API int __stdcall StopIntelligentSearch(HANDLE hChannelHandle);

// set\get Encry info

/*
function:		Set Encryption Info
uiExtIndexDSP:	in:		dsp index,
						0 ~ N-1, N is the total number of proper DSP
pEncryInfo:		in:		addr storing the enc info used to write to dsp
						must not be NULL
ret value:		0 succeed, -1 fail
*/
SYSSDK_API int __stdcall SetEncryInfo(UINT uiExtIndexDSP, ENCRY_INFO * pEncryInfo);

/*
function:		Get Encryption Info
uiExtIndexDSP:	in:		dsp index,
						0 ~ N-1, N is the total number of proper DSP
pEncryInfo:		out:	addr to store the enc info
						must not be NULL
ret value:		0 succeed, -1 fail
*/
SYSSDK_API int __stdcall GetEncryInfo(UINT uiExtIndexDSP, ENCRY_INFO * pEncryInfo);

/*
function:		Get Encryption Info with encryption method, use this function to protect your software
uiExtIndexDSP:	in:		dsp index,
						0 ~ N-1, N is the total number of proper DSP
pEncryInfo:		in:		addr storing the encrypted 8-byte-length password
						must not be NULL
pEncryInfo:		out:	addr to store the encrypted info,
						user use the (de)encryption lib(provided by manufacture) to get the real info
						must not be NULL
ret value:		0 succeed, -1 fail
remarks:		process as follows:
				8-byte-length password --> DH_Encode -->128-byte-length password
				--> GetEncryInfoEx --> 128-byte-length encrypted info --> DH_Decode
				--> 24-byte-length user info
*/
SYSSDK_API int __stdcall GetEncryInfoEx(UINT uiExtIndexDSP, UCHAR * pEncryInfo);
//-----------------------------------------------------------------------

/*
function:		Set OSD Display Mode Extend
hChannel:		in:		channel handle returned by function ChannelOpen()
Brightness:		in:		osd brightness, 0(dark) ~ 255
Translucent:	in:		transparent mark
						TRUE: transparent display
						FALSE: no transparent display
param:			in:		param
						bit0: auto brightness reverse, 1: YES, 0: NO
						bit16~23: vertical zoom factor, 0: automatic, 1: x1, 2: x2, 4: x4
						bit24~31: horizonal zoom factor, 0: automatic, 1: x1, 2: x2, 4: x4
nLineCount:		in:		osd string count,
						0 ~ 8
Format:			in:		formatted string arry, must not be NULL
						formatted string defines:
						USHORT X£¬ USHORT Y£¬ CHAR0£¬ CHAR1£¬ CHAR2£¬¡­ CHARN
						x: start x potion
						y: start y potion
						CHAR0~CHAR1: may be ASCII, UNICODE or defined MACRO(refer to osd_time_format enum struct)
ret value:		0 succeed, -1 fail
*/
SYSSDK_API int __stdcall SetOsdDisplayModeEx(HANDLE hChannel, int Brightness, BOOL Translucent, int param, int nLineCount, USHORT **Format);

/*
function:		send user custom cmd
cmdcode:		in:		cmd code
						0x81:	ask the sdk to identify the board oem id, 
								this function must be used in conjunction with the (de)encryption lib(provided by manufacture)
								must be called before InitDSPs.
								process as follows:
								first call DH_Encode -->128-byte-length infor
								then call DoCustomCmd pass 0x81 as the first param, and infor the second
						0x82:	ask the sdk not to use MessageBox to display error info
						0x83:	ask the sdk not to use default Windows system path to store the config file and log file
						0x84:	ask the sdk not to use sdk render,this will cause failure of StartVideoPreview.

						else:	now do nothing

pinbuf:			in:		input buffer of user data
						if cmdcode == 0x81, this buff stores the encrypted info get by DH_Encode, and the length is 0x80
						if cmdcode == 0x82, this buff is not used
						if cmdcode == 0x83, this buff is not used
						if cmdcode == 0x84, this buff is not used

inbuflen:		in:		input buffer length
						if cmdcode == 0x81, inbuflen must be equal with 0x80.
						if cmdcode == 0x82, inbuflen is ignored
						if cmdcode == 0x83, inbuflen is ignored
						if cmdcode == 0x84, inbuflen is ignored

poutbuf:		out:	output buffer to store returned data

outbuflen:		in:		output buffer length

retlen			out:	tells the actual data length in poutbuf	
*/
SYSSDK_API int __stdcall DoCustomCmd(ULONG cmdcode, BYTE* pinbuf, ULONG inbuflen, BYTE* poutbuf, ULONG outbuflen, ULONG* retlen);

/*
function:		Get sound level, Used to Alarm (sound)
hChannel:		in:		channel handle returned by function ChannelOpen()
ret value:		-1 if failed, actual sound level (0~65535)                                                                             
*/
SYSSDK_API int __stdcall GetSoundLevel(HANDLE hChannel);

/*
function:		set display standard
nDisplayChannel: in:	display channel index
VideoStandard:	 in:	standard to set,see definition of VideoStandard_t
ret value:		0 succeed, -1 fail
*/
SYSSDK_API int __stdcall SetDisplayStandard(UINT nDisplayChannel, UINT Port, VideoStandard_t VideoStandard, UINT FrameRate);

/*
function:		set display region position
nDisplayChannel: in:	display channel index
nRegion:		 in:	region index to fill
nLeft:			 in:	x position, 0 ~ 703
nTop:			 in:	y position, 0 ~ 575
ret value:		0 succeed, -1 fail
*/
SYSSDK_API int __stdcall SetDisplayRegionPosition(UINT nDisplayChannel, UINT nRegion, UINT nLeft, UINT nTop);

/*
function:		fill display region,note: if this region is displaying, this function has no effect.
nDisplayChannel: in:	display channel index
nRegion:		 in:	region index to fill
pImage:			 in:	in buffer of image, content must be consistent with the actual region size, or the
						display may be incorrect.
ret value:		0 succeed, -1 fail
*/
SYSSDK_API int __stdcall FillDisplayRegion(UINT nDisplayChannel, UINT nRegion, unsigned char *pImage);

/*-----------------------------------------------------------------------
	error code
-----------------------------------------------------------------------*/
enum sys_sdk_err
{
	SS_SUCCESS = 0,									// Return Succeed

	ERR_INIT_DSP = 0xd0000001,						// Error When Initiat DSP

	ERR_WAIT_TIMEOUT	=	0xc0000001,				// SDK Operation Time Out

	ERR_INVALID_HANDLE,								// Used Invalid Handle

	ERR_INVALID_ARGUMENT,				        	// Used Invalid Argument

	ERR_DDRAW_CREATE_FAILED,						// Error Return by DDRAW, Refer to MSDN

	ERR_DDRAW_CAPS_FAULT,							// Error Return by DDRAW, Refer to MSDN

	ERR_SET_COOPERATIVELEVEL_FAILED,				// Error Return by DDRAW, Refer to MSDN

	ERR_PRIMARY_SURFACE_CREATE_FAILED,				// Error Return by DDRAW, Refer to MSDN

	ERR_GET_OVERLAY_ADDRESS_FAILED,					// Error Return by DDRAW, Refer to MSDN

	ERR_OVERLAY_SURFACE_CREATE_FAILED,				// Error Return by DDRAW, Refer to MSDN

	ERR_OVERLAY_UPDATE_FAILED,						// Error Return by DDRAW, Refer to MSDN

	ERR_CHANNELMAGIC_MISMATCH,						// Image Data Mismatched

	ERR_CALLBACK_REGISTERED,						// Attempt to Register a Callback Function Twice

	ERR_QUEUE_OVERFLOW,								// Data Queue Overflow

	ERR_STREAM_THREAD_FAILURE,				        // Failed to Start the Stream Thread

	ERR_THREAD_STOP_ERROR,					        // Error when Stop the Stream Thread

	ERR_NOT_SUPPORT,								// The Function is not Currently Supported

	ERR_OUTOF_MEMORY,								// InSufficient System Memory

	ERR_DSP_BUSY,									// DSP is Busy

	ERR_DATA_ERROR,									// Data Error (Must Restart Video Capture)

	ERR_KERNEL,										// Kernel Error

	ERR_OFFSCREEN_CREATE_FAILED,		        	// Failed to Create Offscreen Surface

	ERR_MULTICLOCK_FAILURE,							// Multiclock Error

	ERR_INVALID_DEVICE,								// Used Invalid Device

	ERR_FILE_NOT_FOUNT,						        // Can not Find the expect File

	ERR_BUFFER_TOO_SMALL,							// Buffer is too small

	ERR_COFF_TYPE_ERROR,							// Wrong Coff File Type

	ERR_COFF_SECTION_ERROR,							// Wrong Coff File Section

	ERR_TMMAN_FAILURE,								// SDK Internal Error

	ERR_DOWNLOAD_DSP,								// Error Occur During Download DSP

	ERR_IOCTL,										// Error Occur During Call DeviceIoControl

	ERR_GETLASTERROR,								// Error Occur During Call Win32 API
};

#define OSD_TIME_ITEMS 7
#define _OSD_BASE	0x9000
enum osd_time_format
{
	_OSD_YEAR4	= _OSD_BASE,
	_OSD_YEAR2	,
	_OSD_MONTH3	,
	_OSD_MONTH2	,
	_OSD_DAY	,
	_OSD_WEEK3	,
	_OSD_CWEEK1	,
	_OSD_HOUR24	,
	_OSD_HOUR12	,
	_OSD_MINUTE	,
	_OSD_SECOND	,

	_OSD_END,
};

enum pixel_format
{
	PIXEL_FORMAT_YV12,
	PIXEL_FORMAT_UYVY,
	PIXEL_FORMAT_YUY2,
	PIXEL_FORMAT_YVU9,
	PIXEL_FORMAT_IF09,

	PIXEL_FORMAT_RGB565,
	PIXEL_FORMAT_RGB555,
	PIXEL_FORMAT_RGB24,
	PIXEL_FORMAT_BGR24,
	PIXEL_FORMAT_RGB32,
	PIXEL_FORMAT_BGR32,

	PIXEL_FORMAT_AUTO,
};

enum blind_type
{
	BLIND_CLOSE,
	BLIND_BLACK = 1,
	BLIND_WHITE,
	BLIND_BOTH,
};

enum video_state
{
	VSTATE_NORMAL,
	VSTATE_LOST,
	VSTATE_BLIND,
};

#endif	// __DHSYSSDK_H__
