#include "stdafx.h"
#include "SettingBasicDlg.h"
#include "SettingDlg.h"



LRESULT CBasicSettingDlg::OnInitDialog( UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/ )
{
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->AddMessageFilter(this);
	pLoop->AddIdleHandler(this);
	UIAddChildWindowContainer(m_hWnd);

	m_diskList = GetDlgItem(IDC_LIST_DISK);
	m_diskList.SetWindowLong(GWL_STYLE, LVS_REPORT | LVS_SINGLESEL | WS_CHILD | WS_VISIBLE );
	m_diskList.SetExtendedListViewStyle(LVS_EX_FULLROWSELECT | LVS_EX_FLATSB);
	m_diskList.AddColumn(L"硬盘盘符", 0);
	m_diskList.AddColumn(L"硬盘大小", 1);
	m_diskList.AddColumn(L"剩余空间", 2);
	m_diskList.AddColumn(L"空间占用率", 3);
	TCHAR szBuf[100];
	memset(szBuf,0,100);
	DWORD len = GetLogicalDriveStrings(sizeof(szBuf)/sizeof(TCHAR),szBuf);
	int i=0;
	for (TCHAR* s = szBuf; *s; s += _tcslen(s)+1)
	{
		LPCTSTR sDrivePath = s;	
		UINT uDriveType = GetDriveType(sDrivePath);
		if (uDriveType == DRIVE_FIXED)
		{
			ULARGE_INTEGER nFreeBytesAvailable;
	        ULARGE_INTEGER nTotalNumberOfBytes;

			if (GetDiskFreeSpaceEx(sDrivePath,
				&nFreeBytesAvailable,
				&nTotalNumberOfBytes,NULL
				))
			{
				CString temp1,temp2,temp3;
				CString temp4 = L"%";
				long long free,total;
				total = nTotalNumberOfBytes.QuadPart/(1024*1024);
				free= nFreeBytesAvailable.QuadPart/(1024*1024);
				temp1.Format(L"%dM",free);
				temp2.Format(L"%dM",total);
				temp3.Format(L"%d",100-free*100/total);
				m_diskList.AddItem(i,0,sDrivePath);
				m_diskList.AddItem(i,1,temp1);
				m_diskList.AddItem(i,2,temp2);
				m_diskList.AddItem(i,3,temp3);
				i++;
			}
		}
	}
	return true;
}

LRESULT CBasicSettingDlg::OnLvnItemchangedListDisk(int /*idCtrl*/, LPNMHDR pNMHDR, BOOL& /*bHandled*/)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	if (pNMLV->uNewState&LVIS_SELECTED) 
	{
		int m_nCurrentSel = m_diskList.GetSelectedIndex();   //获得点击的那个行
		m_diskList.GetItemText(m_nCurrentSel,0,m_parent->m_capturePath);
	}	
	return 0;

}

LRESULT CBasicSettingDlg::OnConfirm( UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/ )
{
	return 0;
}
