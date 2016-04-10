#pragma once

#include "afxwin.h"
#include "Resource.h"

#define WM_PRINT_TEXT_MESSAGE     (WM_USER+WM_MAXITEMS+WM_MAXITEMS+WM_MAXITEMS+1)

// CDialogConsole dialog

class CDialogConsole : public CDialogEx
{
	DECLARE_DYNAMIC(CDialogConsole)

public:
	CDialogConsole(CString window_title, CWnd* pParent = NULL);   // standard constructor
	virtual ~CDialogConsole();

// Dialog Data
	enum { IDD = IDD_DIALOG_CONSOLE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

	// Sends a message to the Scintilla editor
	LRESULT SendEditor(UINT Msg, WPARAM wParam = 0, LPARAM lParam = 0)
	{
		return ::SendMessage(m_hwndEditWindow, Msg, wParam, lParam);
	}

	// Sets a Scintilla style
	void SetStyle(int style, COLORREF fore, COLORREF back = RGB(255, 255, 255), int size = -1, const TCHAR* face = 0)
	{
		SendEditor(SCI_STYLESETFORE, style, fore);
		SendEditor(SCI_STYLESETBACK, style, back);
		if (size >= 1)
			SendEditor(SCI_STYLESETSIZE, style, size);
		if (face)
			SendEditor(SCI_STYLESETFONT, style, (LPARAM)face);
	}

public:
	virtual BOOL Create(UINT nIDTemplate, CWnd* pParentWnd = NULL);
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	afx_msg void OnSize(UINT nType, int cx, int cy);

	LRESULT PrintText(WPARAM wParam, LPARAM lParam);
	BOOL IsInitialized() { return m_bInited; }

	CString m_strWindowTitle;
	BOOL m_bVisible;

private:
	void InitialiseEditor();
	void SizeEditor();

	HWND m_hwndEditWindow;
	CString m_strConsoleText;
	BOOL m_bInited;
};
