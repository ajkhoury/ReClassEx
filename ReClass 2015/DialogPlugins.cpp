// DialogPlugins.cpp : implementation file
//

#include "stdafx.h"
#include "DialogPlugins.h"
#include "ReClass2015.h"
#include "afxdialogex.h"

// CDialogPlugins dialog

IMPLEMENT_DYNAMIC(CDialogPlugins, CDialogEx)

CDialogPlugins::CDialogPlugins(CWnd* pParent) 
	: CDialogEx(CDialogPlugins::IDD, pParent)
{

}

CDialogPlugins::~CDialogPlugins()
{

}

void CDialogPlugins::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CDialogPlugins, CDialogEx)

END_MESSAGE_MAP()


// CDialogPlugins message handlers
