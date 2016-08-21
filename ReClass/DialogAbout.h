#pragma once

#include "afxwin.h"

// CAboutDlg dialog used for App About
class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD) { }
	enum { IDD = IDD_ABOUTBOX }; // Dialog Data
protected: // DDX/DDV support
	virtual void CAboutDlg::DoDataExchange(CDataExchange* pDX) 
	{
		CDialogEx::DoDataExchange(pDX);
	}

	virtual BOOL OnInitDialog()
	{
		CDialogEx::OnInitDialog();

		CString cstring_temp;
		cstring_temp.Format(_T("Build Date: %s - %s"), _T(__DATE__), _T(__TIME__));
		SetDlgItemText(IDC_ABOUT_BUILD_DATE, cstring_temp);
		cstring_temp.Format(_T("Version: %s"), RECLASS_VERSION);
		SetDlgItemText(IDC_ABOUT_BUILD_VERSION, cstring_temp);
		return TRUE;
	}

	DECLARE_MESSAGE_MAP() // Implementation
};

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()
