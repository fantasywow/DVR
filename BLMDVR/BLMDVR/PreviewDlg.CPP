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
	temp.Format(L"%d",m_channelID);
	GetDlgItem(IDC_CHANNEL_ID).SetWindowText(temp);
	GetDlgItem(IDC_CHANNEL_ID).SetWindowPos(NULL,10,0,20,20,SWP_SHOWWINDOW);
	return true;
}

LRESULT CPreiviewDlg::OnBnClickedCapturePicture(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	GetParent().SendMessage(BM_CAPTURE_PICTURE,0,(LPARAM)m_channelID);
	return true;
}

//��Ԥ�����ڴ�С�����仯ʱ�ı�Ԥ�����ڵ��а�ťλ��
LRESULT CPreiviewDlg::OnSizeChanged(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	CRect rc;
	GetWindowRect(rc);
	GetDlgItem(IDC_CAPTURE_PICTURE).SetWindowPos(NULL,rc.Width()-40,0,20,20,SWP_SHOWWINDOW);
	return true;

}

void CPreiviewDlg::SetCaptureIcon(bool captureOn)
{
	//����¼��״̬ͼ��
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
	test.Format(L"unfocus:%d",m_channelID);
	ATLTRACE(test);
	unfocus();
	return true;
}

void CPreiviewDlg::focus()
{
	//����ѡ�б߿�
}

void CPreiviewDlg::unfocus()
{
	//ɾ��ѡ�б߿�
}