#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// DialogProgress dialog

class CDialogProgress : public CDialogEx
{
	DECLARE_DYNAMIC(CDialogProgress)

public:
	CDialogProgress(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDialogProgress();

// Dialog Data
	enum { IDD = IDD_DIALOG_PROGRESS };

	CProgressCtrl& Bar( ) { return m_progressBar; }
	void SetProgressText(const CString& str) { m_progressText.SetWindowText(str); }

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

private:
	CProgressCtrl m_progressBar;
	CStatic m_progressText;
};
