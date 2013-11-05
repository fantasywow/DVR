#include "stdafx.h" 
#include "BlmSerialComm.h"

BlmSerialComm::BlmSerialComm( void )   
{   
	m_hCom = 0;   
	m_lpszErrorMessage[0] = '\0';   
	ZeroMemory(&m_ov, sizeof(m_ov));   
}   

BlmSerialComm::BlmSerialComm( LPWSTR lpszPortNum )   
{    
	BlmSerialComm::BlmSerialComm();   
	BlmSerialComm::open( lpszPortNum );    
}   


BlmSerialComm::~BlmSerialComm() {   
	close();   
}   



char*   BlmSerialComm::GetErrorMessage( void ) {   
	return m_lpszErrorMessage;   
}   


int BlmSerialComm::open( CString lpszPortNum,    
						DWORD  dwBaudRate,    
						BYTE   byParity,   
						BYTE   byStopBits,   
						BYTE   byByteSize ) {   
							DCB  dcb;   //�����豸���ƿ� Device Control Block   
							BOOL bSuccess;   

							// m_hCom��Ϊ�������صĴ��ڵľ��   
							m_hCom = CreateFile( lpszPortNum,           // pointer to name of the file   
								GENERIC_READ|GENERIC_WRITE, // �����д��   
								0,                     // ͨѶ�豸�����Զ�ռ��ʽ�򿪡�   
								NULL,                  // �ް�ȫ���ԣ���ʾ�ô��ڲ���   
								// ���ӳ���̳С�   
								OPEN_EXISTING,         // ͨѶ�豸�Ѵ��ڡ�   
								FILE_FLAG_OVERLAPPED,  // ʹ���첽��ʽ overlapped I/O��   
								NULL);                 // ͨѶ�豸������ģ��򿪡�   
							if ( m_hCom == INVALID_HANDLE_VALUE ) {   
								BlmSerialComm::ErrorToString("RS232::open()   CreateFile() failed, invalid handle value");   
								return FALSE;   
							}   

							// �봮����صĲ����ǳ��࣬����Ҫ���ô��ڲ���ʱ��ͨ������ȡ�ô���   
							// �Ĳ����ṹ���޸Ĳ��ֲ������ٽ������ṹд��   
							bSuccess = GetCommState(m_hCom, &dcb);   
							if ( !bSuccess ) {   
								BlmSerialComm::close();   
								BlmSerialComm::ErrorToString("RS232::open()   GetCommState() failed");   
								return FALSE;   
							}   
							dcb.BaudRate = dwBaudRate;   // ���ڲ����ʡ�   
							dcb.Parity   = byParity;     // У�鷽ʽ��ֵ0~4�ֱ��Ӧ��У�顢��   
							// У�顢żУ�顢У�顢��λ��У�����㡣    
							dcb.fParity = 0;             // Ϊ1�Ļ�������żУ���顣   
							dcb.ByteSize = byByteSize;   // һ���ֽڵ�����λ��������Χ��5~8��   
							dcb.StopBits = byStopBits;   // ֹͣλ������0~2�ֱ��Ӧ1λ��1.5λ��   
							// 2λֹͣλ��   
							if ( !bSuccess ) {   
								BlmSerialComm::close();   
								BlmSerialComm::ErrorToString("RS232::open()   SetCommState() failed");   
								return FALSE;   
							}   

							return TRUE;   
}   


DWORD BlmSerialComm::output( LPCVOID pdata, DWORD   len ) {   
	BOOL  bSuccess;   
	DWORD written = 0;   

	if ( len < 1 )   
		return 0;   
	// create event for overlapped I/O   
	m_ov.hEvent = CreateEventA( NULL,   // pointer to security attributes    
		FALSE,  // flag for manual-reset event    
		FALSE,  // flag for initial state    
		"");    // pointer to event-object name    
	if ( m_ov.hEvent == INVALID_HANDLE_VALUE ) {   
		BlmSerialComm::ErrorToString( "RS232::output()   CreateEvent() failed" );   
		return -1;   
	}   
	bSuccess = WriteFile( m_hCom,   // handle to file to write to     
		pdata,    // pointer to data to write to file    
		len,      // number of bytes to write    
		&written, // pointer to number of bytes written    
		&m_ov );  // pointer to structure needed for overlapped I/O   
	// �������ִ�гɹ��Ļ����written��ֵΪд����ֽ�����WriteFile����ִ����Ϻ�    
	// �������ģ����ô˱��������ֵ�����������ú����Ƿ����е����ݳɹ�д�봮��   
	if ( BlmSerialComm::IsNT() ) {   
		bSuccess = GetOverlappedResult( m_hCom, &m_ov, &written, TRUE );   
		if ( !bSuccess ) {   
			CloseHandle( m_ov.hEvent );   
			BlmSerialComm::ErrorToString( "RS232::output()   GetOverlappedResult() failed" );   
			return -1;   
		}   
	}   
	else if ( len != written ) {   
		CloseHandle( m_ov.hEvent );   
		BlmSerialComm::ErrorToString( "RS232::output()   WriteFile() failed" );   
		return -1;   
	}   
	CloseHandle( m_ov.hEvent );   
	return written;   
}   



DWORD BlmSerialComm::input( LPVOID pdest, DWORD  len, DWORD  dwMaxWait ) {   
	BOOL  bSuccess;   
	DWORD result = 0,   
		read   = 0, // num read bytes   
		mask   = 0; // a 32-bit variable that receives a mask    
	// indicating the type of event that occurred   
	if ( len < 1 )   
		return(0);   
	// create event for overlapped I/O   
	m_ov.hEvent = CreateEventA( NULL,    // pointer to security attributes    
		FALSE,   // flag for manual-reset event    
		FALSE,   // flag for initial state    
		"" );    // pointer to event-object name    
	if ( m_ov.hEvent == INVALID_HANDLE_VALUE ) {   
		BlmSerialComm::ErrorToString( "RS232::input()   CreateEvent() failed" );   
		return -1;   
	}   
	// Specify here the event to be enabled   
	bSuccess = SetCommMask( m_hCom, EV_RXCHAR );   
	if ( ! bSuccess ) {   
		CloseHandle(m_ov.hEvent);   
		BlmSerialComm::ErrorToString("RS232::input()   SetCommMask() failed");   
		return -1;   
	}   
	// WaitForSingleObject   
	bSuccess = WaitCommEvent(m_hCom, &mask, &m_ov);   
	if ( !bSuccess ) {   
		int err = GetLastError();   
		if ( err == ERROR_IO_PENDING ) {   
			result = WaitForSingleObject(m_ov.hEvent, dwMaxWait);  //wait dwMaxWait   
			// milli seconds before returning   
			if ( result == WAIT_FAILED ) {   
				CloseHandle(m_ov.hEvent);   
				BlmSerialComm::ErrorToString( "RS232::input()   WaitForSingleObject() failed" );   
				return -1;   
			}   
		}   
	}   
	// The specified event occured?   
	if ( mask & EV_RXCHAR )    
	{   
		bSuccess = ReadFile( m_hCom, // handle of file to read    
			pdest,  // address of buffer that receives data    
			len,    // number of bytes to read    
			&read,  // address of number of bytes read    
			&m_ov); // address of structure for data    
		if ( BlmSerialComm::IsNT() ) {   
			bSuccess = GetOverlappedResult(m_hCom, &m_ov, &read, TRUE);   
			if ( !bSuccess ) {   
				CloseHandle( m_ov.hEvent );   
				BlmSerialComm::ErrorToString( "RS232::input()   GetOverlappedResult() failed" );   
				return -1;   
			}   
		}   
		else if ( !bSuccess ) {   
			CloseHandle(m_ov.hEvent);   
			BlmSerialComm::ErrorToString( "RS232::input()   ReadFile() failed" );   
			return -1;   
		}   
	}   
	else {   
		CloseHandle(m_ov.hEvent);   
		wsprintfA( m_lpszErrorMessage, "RS232::input()   No EV_RXCHAR occured\n" );   
		return -1;   
	}   
	CloseHandle(m_ov.hEvent);   
	return read;   
}   


void BlmSerialComm::close( void ) {   
	if ( m_hCom > 0 )   
		CloseHandle( m_hCom );   
	m_hCom = 0;    
}   


VOID BlmSerialComm::ErrorToString( char* lpszMessage ) {   
	LPVOID lpMessageBuffer;   
	DWORD  error = GetLastError();   

	FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER |   
		FORMAT_MESSAGE_FROM_SYSTEM,      // source and processing options   
		NULL,                            // pointer to message source   
		error,                           // requested message identifie   
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // the user default language.   
		( LPTSTR ) &lpMessageBuffer,     // pointer to message buffer   
		0,                               // maximum size of message buffer   
		NULL);                           // address of array of message inserts    

	// and copy it in our error string   
	wsprintfA(m_lpszErrorMessage,"%s: (%d) %s\n", lpszMessage, error, lpMessageBuffer);   

	LocalFree(lpMessageBuffer);   
}   


BOOL BlmSerialComm::IsNT( void ) {   
	OSVERSIONINFO osvi;   

	osvi.dwOSVersionInfoSize = sizeof( OSVERSIONINFO );   
	GetVersionEx( &osvi );   
	if ( osvi.dwPlatformId == VER_PLATFORM_WIN32_NT )   
		return TRUE;   
	else   
		return FALSE;   
}   
