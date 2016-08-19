// DialogProgress.cpp : implementation file
//

#include "stdafx.h"
#include "DialogProgress.h"
#include "afxdialogex.h"


// CDialogProgress dialog

IMPLEMENT_DYNAMIC(CDialogProgress, CDialogEx)

CDialogProgress::CDialogProgress(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_PROGRESS, pParent)
{

}

CDialogProgress::~CDialogProgress()
{
}

void CDialogProgress::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESSBAR, m_progressBar);
}

BEGIN_MESSAGE_MAP(CDialogProgress, CDialogEx)
END_MESSAGE_MAP()


// CDialogProgress message handlers
BOOL CDialogProgress::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	m_progressBar.SetRange(0, 100);
	//m_progressBar.SetStep(2);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CDialogProgress::SetProgress(int pos)
{
	return m_progressBar.SetPos(pos);
}
