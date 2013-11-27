#include "stdafx.h"
#include "RecordManager.h"
#include "CppSQLite3.h"
#include <string>
#include <sstream>
using namespace std;

CppSQLite3DB m_sqllitedb;

static const string RECORD_TABLENAME = "t_record";

void LookUpbyMonth(SYSTEMTIME st,LPMONTHDAYSTATE prgDayState)
{
	int begintime = st.wYear*10000+st.wMonth*100;
	int endtime = st.wYear*10000+st.wMonth*100+31;
	prgDayState[0] = 0;
	stringstream ss;
	ss<<"select date from ";
	ss<<RECORD_TABLENAME;
	ss<<" where date>";
	ss<<begintime;
	ss<<" and date<";
	ss<<endtime;
	ss<<";";

	CppSQLite3Query q = m_sqllitedb.execQuery(ss.str().c_str());

	while(!q.eof())
	{
		prgDayState[0] |= 0x01<<(q.getIntField(0)%100-1);
		q.nextRow();
	}

	q.finalize();
	
}
void LookUpbyDay( SYSTEMTIME st,vector<BlmRecord>& record)
{
	record.clear();
	int date = st.wYear*10000+st.wMonth*100+st.wDay;
	stringstream ss;
	ss<<"select filePath,channel,begintime,endtime from ";
	ss<<RECORD_TABLENAME;
	ss<<" where date=";
	ss<<date;
	ss<<" and exist=";
	ss<<1;
	ss<<" and sub=";
	ss<<0;
	ss<<" order by begintime";
	ss<<";";

	CppSQLite3Query q = m_sqllitedb.execQuery(ss.str().c_str());

	while(!q.eof())
	{	
		BlmRecord oneRecord;
		oneRecord.fileName = q.fieldValue(0);
		oneRecord.channel = q.getIntField(1);
		oneRecord.beginTime = q.getIntField(2);
		oneRecord.endTime = q.getIntField(3);
		record.push_back(oneRecord);
		q.nextRow();
	}
	q.finalize();

}

void InsertOneRecord( int channel,int sub,SYSTEMTIME st,CString fileFullPathandName )
{
	int date = st.wYear*10000+st.wMonth*100+st.wDay;
	int begintime = st.wHour*10000+st.wMinute*100+st.wSecond;
	string fileName = fileFullPathandName.GetBuffer(0);
	stringstream ss;
	ss<<"insert into ";
	ss<<RECORD_TABLENAME;
	ss<<" (filePath,date,channel,begintime,endtime,exist,sub) values";
	ss<<"(";
	ss<<"'"<<fileFullPathandName<<"',";
	ss<<date<<",";
	ss<<channel<<",";
	ss<<begintime<<",";
	ss<<0<<","; //endtime
	ss<<1<<","; //exist
	ss<<sub;  //sub
	ss<<");";
	m_sqllitedb.execDML(ss.str().c_str());
}


bool InitRecordIndex()
{
	string fileName="recorddb.db";
	HANDLE fileHandle = CreateFile(fileName.c_str(),GENERIC_READ | GENERIC_WRITE,NULL,NULL,CREATE_NEW,FILE_ATTRIBUTE_NORMAL,NULL);
	CloseHandle(fileHandle);
	try
	{
		m_sqllitedb.open(fileName.c_str());
	}
	catch(...)
	{
		return false;
	}
	if(!(m_sqllitedb.tableExists(RECORD_TABLENAME.c_str())))
	{
		string strsql = "create table "+ RECORD_TABLENAME
				+"(filePath varchar2(256) primary key,date int,channel int,begintime int,endtime int,exist int,sub int);";
		m_sqllitedb.execDML(strsql.c_str());
	}


// 	{ //for test
//  		SYSTEMTIME  st;
//  		GetLocalTime(&st);
//  		CString test;
// 		for (int i =0;i<5;i++)
// 		{
// 			test.Format("testtest%d",i);
// 			st.wMinute=0;
// 			st.wHour++;
// 			InsertOneRecord(0,0,st,test);
// 			st.wMinute = 30;
// 			updateEndTime(st,test);
// 		}
//  		InsertOneRecord(0,0,st,test);
//  		updateEndTime(st,test);
// // 		MONTHDAYSTATE prgDayState;
// // 		LookUpbyMonth(st,&prgDayState);
// 		vector<BlmRecord> temp;
// 		missingOneRecord(test);
// 		LookUpbyDay(st,temp);
// 	}

	return true;
}

void updateEndTime(SYSTEMTIME st,CString fileFullPathandName )
{
	int endtime = st.wHour*10000+st.wMinute*100+st.wSecond;
	string fileName = fileFullPathandName.GetBuffer(0);
	stringstream ss;
	ss<<"update ";
	ss<<RECORD_TABLENAME;
	ss<<" set endtime = "<<endtime;
	ss<<" where filePath=";
	ss<<"'"+fileFullPathandName+"'";     
	m_sqllitedb.execDML(ss.str().c_str()); 

}

void missingOneRecord( CString fileFullPathandName )
{
	string fileName = fileFullPathandName.GetBuffer(0);
	stringstream ss;
	ss<<"update ";
	ss<<RECORD_TABLENAME;
	ss<<" set exist = "<<0;
	ss<<" where filePath=";
	ss<<"'"+fileFullPathandName+"'";     
	m_sqllitedb.execDML(ss.str().c_str()); 

}

