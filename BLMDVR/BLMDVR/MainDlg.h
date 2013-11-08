// MainDlg.h : interface of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

#include "dhvecsystem.h"
#include "PreviewDlg.h"
#include "resource.h"
#include "BlmMessage.h"
#include "SettingDlg.h"
#include "BlmTypes.h"

enum PreviewLayout{
	PREVIEWLAYOUTFULL = 0,
	PREVIEWLAYOUT1    = 1,
	PREVIEWLAYOUT4    = 4,
	PREVIEWLAYOUT8    = 8
};

class CMainDlg : public CDialogImpl<CMainDlg>, public CUpdateUI<CMainDlg>,
		public CMessageFilter, public CIdleHandler
{


private:
	HANDLE m_channelHandle[CHANNEL_MAX];\
	CTrackBarCtrl m_brightnessSlide;
	CTrackBarCtrl m_contrastSlide;
	CTrackBarCtrl m_saturationSlide;
	CTrackBarCtrl m_hueSlide;
	CPreiviewDlg m_previewDlg[CHANNEL_MAX];
	CSettingDlg m_settingDlg;
	BOOL m_isFullScreen;
	PreviewLayout m_lastLayout;
	int m_focusChannel;
	void SetPreviewDlgLayout(PreviewLayout layout,int channelID);
	void FocusChannel(int channelID);
public:
	enum { IDD = IDD_MAINDLG };

	BlmUserSetting m_setting;

	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnIdle();

	BEGIN_UPDATE_UI_MAP(CMainDlg)
	END_UPDATE_UI_MAP()

	BEGIN_MSG_MAP(CMainDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		MESSAGE_HANDLER(BM_CAPTURE_PICTURE, OnCapturePicture)
		MESSAGE_HANDLER(BM_PREVIEW_DBLCLK, OnPreviewDBLCLK)
		MESSAGE_HANDLER(BM_PREVIEW_FOCUS, OnPreviewFocus)
		COMMAND_ID_HANDLER(IDCANCEL, OnClose)
		COMMAND_HANDLER(IDC_LAYOUTBUTTON1, BN_CLICKED, OnBnClickedLayoutbutton1)
		COMMAND_HANDLER(IDC_LAYOUTBUTTON4, BN_CLICKED, OnBnClickedLayoutbutton4)
		COMMAND_HANDLER(IDC_LAYOUTBUTTON8, BN_CLICKED, OnBnClickedLayoutbutton8)
		COMMAND_HANDLER(IDC_SETTINGBUTTON, BN_CLICKED, OnBnClickedSettingbutton)
		MESSAGE_HANDLER(WM_TIMER, OnTimer)
		MESSAGE_HANDLER(WM_HSCROLL, OnHScroll)
	END_MSG_MAP()

// Handler prototypes (uncomment arguments if needed):
//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnCapturePicture(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnClose(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	void CloseDialog(int nVal);

	LRESULT OnBnClickedLayoutbutton1(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedLayoutbutton4(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedLayoutbutton8(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnPreviewDBLCLK(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnPreviewFocus(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedSettingbutton(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnTimer(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnHScroll(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
};
