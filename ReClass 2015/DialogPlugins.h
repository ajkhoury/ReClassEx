#pragma once

#include "afxwin.h"
#include "Resource.h"

// CDialogPlugins dialog

class CDialogPlugins : public CDialogEx
{
	DECLARE_DYNAMIC(CDialogPlugins)

public:
	CDialogPlugins(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDialogPlugins();

// Dialog Data
	enum { IDD = IDD_DIALOG_PLUGINS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
