

void LookUpbyMonth(SYSTEMTIME st,LPMONTHDAYSTATE prgDayState);
void LookUpbyDay(SYSTEMTIME st,BOOL *hasRecord);
void InsertOneRecord(SYSTEMTIME st,CString fileFullPathandName);
bool InitRecordXMLfile();