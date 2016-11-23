#pragma once

#include "afxwin.h"

class CDialogEdit : public CDialogEx
{
	DECLARE_DYNAMIC(CDialogEdit)
public:
	CDialogEdit(CWnd* pParent = NULL);
	virtual ~CDialogEdit();

	CString Title;
	CString Text;

	// Sends a message to the Scintilla editor
	LRESULT SendEditor( UINT Msg, WPARAM wParam = 0, LPARAM lParam = 0 );

	// Sets a Scintilla style
	void SetAStyle( int style, COLORREF fore, COLORREF back = RGB( 255, 255, 255 ), int size = -1, const TCHAR* face = 0 );

	void InitialiseEditor();
	void SizeEditor();

	HWND m_hwndEditor;

	enum { IDD = IDD_DIALOG_EDIT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

public:
	virtual BOOL Create(LPCTSTR lpszTemplateName, CWnd* pParentWnd = NULL);
	virtual BOOL OnInitDialog();
	virtual void OnCancel();

	DECLARE_MESSAGE_MAP()
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnFileEditoropen();
	afx_msg void OnFileEditorsaveas();
};
