// DialogProgress.cpp : implementation file
//

#include "stdafx.h"
#include "DialogProgress.h"

#include <afxdialogex.h>


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
    
    // TODO:  Add extra initialization here
    return TRUE;  // return TRUE unless you set the focus to a control
                  // EXCEPTION: OCX Property Pages should return FALSE
}

