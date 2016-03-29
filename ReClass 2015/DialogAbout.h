#pragma once

#include "afxwin.h"

// CAboutDlg dialog used for App About
class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD) { }
	enum { IDD = IDD_ABOUTBOX }; // Dialog Data
protected: // DDX/DDV support
	virtual void CAboutDlg::DoDataExchange(CDataExchange* pDX) { CDialogEx::DoDataExchange(pDX); }
	DECLARE_MESSAGE_MAP() // Implementation
};
BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()
