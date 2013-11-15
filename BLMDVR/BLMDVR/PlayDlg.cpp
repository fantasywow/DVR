#include "stdafx.h"
#include "PlayDlg.h"
#include "dhplay.h"




LRESULT CPlayDlg::OnInitDialog( UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/ )
{
	SetWindowPos(NULL,0,0,600,400,SWP_SHOWWINDOW);
	CenterWindow();
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->AddMessageFilter(this);
	pLoop->AddIdleHandler(this);
	UIAddChildWindowContainer(m_hWnd);

	RECT rect = {124, 0, 124+352, 288};
	m_playWindow.Create(m_hWnd, rect ,NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, WS_EX_CLIENTEDGE);		
	
	PLAY_GetFreePort(&m_port);
	PLAY_OpenFile(m_port,m_filePath);
	PLAY_Play(m_port,m_playWindow.m_hWnd);

	return TRUE;
}

LRESULT CPlayDlg::OnDestroy( UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/ )
{
	// unregister message filtering and idle updates
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->RemoveMessageFilter(this);
	pLoop->RemoveIdleHandler(this);

	return 0;
}

LRESULT CPlayDlg::OnClose(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	//EndDialog(0);
	DestroyWindow();
	return 0;
}
