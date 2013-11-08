
/////////////////////////////////////////////////////////////////////////////
#pragma once

#include "resource.h"
#include "BlmMessage.h"
#include "SettingEncodeDlg.h"
#include "SettingBasicDlg.h"
#include "SettingCradleDlg.h"
#include "SettingPictureDlg.h"
#include "SettingRecordDlg.h"
#include "BlmTypes.h"

class CSettingDlg : public CDialogImpl<CSettingDlg> ,
	public CUpdateUI<CSettingDlg>,
	public CMessageFilter, 
	public CIdleHandler
{
private:
	CEncodeSettingDlg m_encodeDlg;
	CPictureSettingDlg m_pictureDlg;
	CBasicSettingDlg  m_basicDlg;
	CRecordSettingDlg m_recodeDlg;
	CCradleSettingDlg m_cradleDlg;

public:
	enum { IDD = IDD_SETTINGDLG };

	BlmUserSetting m_setting;

	virtual BOOL PreTranslateMessage(MSG* pMsg){
		return CWindow::IsDialogMessage(pMsg);
	}
	virtual BOOL OnIdle(){
		return FALSE;
	}

	BEGIN_UPDATE_UI_MAP(CSettingDlg)
	END_UPDATE_UI_MAP()

	BEGIN_MSG_MAP(CSettingDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_ID_HANDLER(IDCANCEL, OnClose)
		COMMAND_HANDLER(IDC_BUTTON_BASIC, BN_CLICKED, OnBnClickedButtonBasic)
		COMMAND_HANDLER(IDC_BUTTON_PICTURE, BN_CLICKED, OnBnClickedButtonPicture)
		COMMAND_HANDLER(IDC_BUTTON_ENCODE, BN_CLICKED, OnBnClickedButtonEncode)
		COMMAND_HANDLER(IDC_BUTTON_CRADLE, BN_CLICKED, OnBnClickedButtonCradle)
		COMMAND_HANDLER(IDC_BUTTON_RECODE, BN_CLICKED, OnBnClickedButtonRecode)

		COMMAND_HANDLER(IDC_CONFIRM, BN_CLICKED, OnBnClickedConfirm)
		COMMAND_HANDLER(IDC_CANCEL, BN_CLICKED, OnBnClickedCancel)
	END_MSG_MAP()

	// Handler prototypes (uncomment arguments if needed):
	//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnClose(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedButtonBasic(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedButtonPicture(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedButtonEncode(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedButtonCradle(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedButtonRecode(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedConfirm(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedCancel(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
};
