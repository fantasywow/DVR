#include "BlmTypes.h"
#include <vector>
using namespace std;

void LookUpbyMonth(SYSTEMTIME st,LPMONTHDAYSTATE prgDayState);
void LookUpbyDay(SYSTEMTIME st,vector<BlmRecord>& record);
void InsertOneRecord(int channel,int sub,SYSTEMTIME st,CString fileFullPathandName);
void updateEndTime(SYSTEMTIME st,CString fileFullPathandName);
bool InitRecordIndex();
void missingOneRecord(CString fileFullPathandName);
