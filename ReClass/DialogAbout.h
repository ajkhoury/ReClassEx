#pragma once

#include "afxwin.h"

// CAboutDlg dialog used for App About
class CDialogAbout : public CDialogEx
{
public:
	CDialogAbout();
	enum { IDD = IDD_ABOUTBOX }; // Dialog Data

protected: // DDX/DDV support
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP() // Implementation
};
