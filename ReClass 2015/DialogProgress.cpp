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
	DDX_Control(pDX, IDC_PROGRESSTEXT, m_progressText);
}

BEGIN_MESSAGE_MAP(CDialogProgress, CDialogEx)
END_MESSAGE_MAP()


// CDialogProgress message handlers
BOOL CDialogProgress::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	
	m_progressBar.SetStep(1);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

void CDialogProgress::SetProgressRange(int range)
{
	m_progressBar.SetRange32(0, range);
}

BOOL CDialogProgress::SetProgressPosition(int pos)
{
	return m_progressBar.SetPos(pos);
}

void CDialogProgress::SetProgressText(CString txt)
{
	m_progressText.SetWindowTextW(txt.GetString());
}

int CDialogProgress::SetStep(int step)
{
	return m_progressBar.SetStep(step);
}

int CDialogProgress::Step()
{
	return m_progressBar.StepIt();
}
