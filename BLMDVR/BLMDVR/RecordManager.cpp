#include "stdafx.h"
#include "RecordManager.h"
#include "tinyxml2.h"


using namespace tinyxml2;

tinyxml2::XMLDocument doc;

void LookUpbyMonth(SYSTEMTIME st,LPMONTHDAYSTATE prgDayState)
{
	char temp[20];
	sprintf(temp,"%d-%d",st.wYear,st.wMonth);
	doc.FirstChildElement(temp);
	prgDayState[0] = 0;
	prgDayState[0] |= 0x01<<5;
}
void LookUpbyDay(SYSTEMTIME st,BOOL* hasRecord)
{
	for (int i = 0;i<BLM_CHANNEL_MAX;i++){
		for(int j =0;j<24;j++){
			hasRecord[i*24+j]=FALSE;
		}	
	}
}

void InsertOneRecord( SYSTEMTIME st,CString fileFullPathandName )
{
	char monthTemp[20];
	char dayTemp[10];
	sprintf(monthTemp,"%d-%d",st.wYear,st.wMonth);
	XMLElement* monthXml= doc.FirstChildElement(monthTemp);
	if (monthXml ==NULL){
		XMLElement* newElement = doc.NewElement(monthTemp);
		doc.InsertEndChild(newElement);
		monthXml= doc.FirstChildElement(monthTemp);
	}
	itoa(st.wDay,dayTemp,10);
	XMLElement* dayXml=doc.FirstChildElement(dayTemp);
	if (dayXml ==NULL)
	{
		XMLElement* newElement = doc.NewElement(dayTemp);
		monthXml->InsertEndChild(newElement);
		dayXml= monthXml->FirstChildElement(dayTemp);
	}

}


bool InitRecordXMLfile()
{
	FILE *fp;
	if(!(fp=fopen("record.xml","r+"))){
		return false;
	}
	doc.LoadFile(fp);
}
