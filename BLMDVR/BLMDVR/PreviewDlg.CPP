#include "StdAfx.h"
#include "PreviewDlg.h"

LRESULT CPreiviewDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/){
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->AddMessageFilter(this);
	pLoop->AddIdleHandler(this);
	UIAddChildWindowContainer(m_hWnd);
	m_channelID=(int)lParam;
	CString temp;
	temp.Format("%d",m_channelID);
	GetDlgItem(IDC_CHANNEL_ID).SetWindowText(temp);
	GetDlgItem(IDC_CHANNEL_ID).SetWindowPos(NULL,10,0,20,20,SWP_SHOWWINDOW);

	m_captureIcon.Create(m_hWnd, CRect(0,0,0,0) ,NULL, WS_CHILD | WS_VISIBLE|SS_BLACKRECT, NULL);		
	SetCaptureIcon(false);
	return true;
}

LRESULT CPreiviewDlg::OnBnClickedCapturePicture(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	GetParent().SendMessage(BM_CAPTURE_PICTURE,0,(LPARAM)m_channelID);
	return true;
}

//当预览窗口大小发生变化时改变预览窗口的中按钮位置
LRESULT CPreiviewDlg::OnSizeChanged(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	CRect rc;
	GetWindowRect(rc);
	GetDlgItem(IDC_CAPTURE_PICTURE).SetWindowPos(NULL,rc.Width()-50,0,20,20,SWP_SHOWWINDOW);
	m_captureIcon.SetWindowPos(NULL,rc.Width()-30,0,20,20,SWP_SHOWWINDOW);

	return true;

}

void CPreiviewDlg::SetCaptureIcon(bool captureOn)
{
	//设置录制状态图标
	if (captureOn)
	{
		//m_captureIcon.SetIcon();
	}else{
		//m_captureIcon.SetIcon();
	}

}
LRESULT CPreiviewDlg::OnLButtonDblClk(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	GetParent().SendMessage(BM_PREVIEW_DBLCLK,0,(LPARAM)m_channelID);
	return true;
}


LRESULT CPreiviewDlg::OnPreviewFocus(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{

	GetParent().SendMessage(BM_PREVIEW_FOCUS,0,(LPARAM)m_channelID);
	focus();
	return 0;
}

LRESULT CPreiviewDlg::OnPreviewUnFocus( UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/ )
{
	CString test;
	test.Format("unfocus:%d",m_channelID);
	ATLTRACE(test);
	unfocus();
	return true;
}

void CPreiviewDlg::focus()
{
	//添加选中边框
}

void CPreiviewDlg::unfocus()
{
	//删除选中边框
}
