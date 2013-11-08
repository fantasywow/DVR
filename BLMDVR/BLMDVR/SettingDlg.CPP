#include "stdafx.h"
#include "SettingDlg.h"


LRESULT CSettingDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/){
	SetWindowPos(NULL,0,0,800,500,SWP_SHOWWINDOW);
	CenterWindow();
	
	m_basicDlg.Create(m_hWnd);
	m_encodeDlg.Create(m_hWnd);
	m_pictureDlg.Create(m_hWnd);
	m_recodeDlg.Create(m_hWnd);
	m_cradleDlg.Create(m_hWnd);
	m_basicDlg.SetWindowPos(NULL,100,0,700,400,SWP_SHOWWINDOW);
	m_encodeDlg.SetWindowPos(NULL,100,0,700,400,SWP_HIDEWINDOW);
	m_pictureDlg.SetWindowPos(NULL,100,0,700,400,SWP_HIDEWINDOW);
	m_recodeDlg.SetWindowPos(NULL,100,0,700,400,SWP_HIDEWINDOW);
	m_cradleDlg.SetWindowPos(NULL,100,0,700,400,SWP_HIDEWINDOW);
	
	return true;
}

LRESULT CSettingDlg::OnClose(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/){
	EndDialog(wID);
	return 0;
}

LRESULT CSettingDlg::OnBnClickedButtonBasic(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	m_basicDlg.ShowWindow(SW_SHOW);
	m_encodeDlg.ShowWindow(SW_HIDE);
	m_pictureDlg.ShowWindow(SW_HIDE);
	m_recodeDlg.ShowWindow(SW_HIDE);
	m_cradleDlg.ShowWindow(SW_HIDE);
	return 0;
}

LRESULT CSettingDlg::OnBnClickedButtonPicture(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	m_basicDlg.ShowWindow(SW_HIDE);
	m_encodeDlg.ShowWindow(SW_HIDE);
	m_pictureDlg.ShowWindow(SW_SHOW);
	m_recodeDlg.ShowWindow(SW_HIDE);
	m_cradleDlg.ShowWindow(SW_HIDE);
	return 0;
}

LRESULT CSettingDlg::OnBnClickedButtonEncode(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	m_basicDlg.ShowWindow(SW_HIDE);
	m_encodeDlg.ShowWindow(SW_SHOW);
	m_pictureDlg.ShowWindow(SW_HIDE);
	m_recodeDlg.ShowWindow(SW_HIDE);
	m_cradleDlg.ShowWindow(SW_HIDE);
	return 0;
}

LRESULT CSettingDlg::OnBnClickedButtonCradle(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	m_basicDlg.ShowWindow(SW_HIDE);
	m_encodeDlg.ShowWindow(SW_HIDE);
	m_pictureDlg.ShowWindow(SW_HIDE);
	m_recodeDlg.ShowWindow(SW_HIDE);
	m_cradleDlg.ShowWindow(SW_SHOW);
	return 0;
}

LRESULT CSettingDlg::OnBnClickedButtonRecode(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	m_basicDlg.ShowWindow(SW_HIDE);
	m_encodeDlg.ShowWindow(SW_HIDE);
	m_pictureDlg.ShowWindow(SW_HIDE);
	m_recodeDlg.ShowWindow(SW_SHOW);
	m_cradleDlg.ShowWindow(SW_HIDE);
	return 0;
}

LRESULT CSettingDlg::OnBnClickedConfirm(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: 添加保存设置的功能
	EndDialog(wID);
	return 0;
}

LRESULT CSettingDlg::OnBnClickedCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	EndDialog(wID);
	return 0;
}
