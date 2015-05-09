#pragma once
#include "afxwin.h"

class CDialogClasses : public CDialogEx
{
	DECLARE_DYNAMIC( CDialogClasses )
public:
	CDialogClasses( CWnd* pParent = NULL );
	virtual ~CDialogClasses( );

	enum { IDD = IDD_DIALOG_CLASSES };

protected:
	virtual void DoDataExchange( CDataExchange* pDX );


	DECLARE_MESSAGE_MAP( )
public:
	CImageList	m_ImageList;
	CListCtrl	m_ClassView;
	CEdit		m_Edit;
	CString		m_Filter;
	HICON		m_hClassIcon;

	virtual BOOL OnInitDialog( );
	virtual void OnOK( );
	virtual void AddData( CListCtrl& ctrl, int row, int col, const char* str );
	virtual void BuildList( );

	afx_msg void OnEnChangeClassname( );
};