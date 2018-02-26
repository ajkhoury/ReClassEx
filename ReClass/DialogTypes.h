#pragma once
#include "afxwin.h"


// CDialogTypes dialog

class CDialogTypes : public CDialogEx {
    DECLARE_DYNAMIC( CDialogTypes )
public:
    CDialogTypes( CWnd* pParent = NULL );   // standard constructor
    virtual ~CDialogTypes( );

    // Dialog Data
    enum { IDD = IDD_DIALOG_TYPES };

protected:
    virtual void DoDataExchange( CDataExchange* pDX );    // DDX/DDV support

    DECLARE_MESSAGE_MAP( )

public:
    virtual BOOL OnInitDialog( );
    virtual void OnOK( );

public:
    CEdit m_Int64;
    CEdit m_Int32;
    CEdit m_Int16;
    CEdit m_Int8;
    CEdit m_Qword;
    CEdit m_Dword;
    CEdit m_Word;
    CEdit m_Byte;
    CEdit m_Vec2;
    CEdit m_Vec3;
    CEdit m_Quat;
    CEdit m_Matrix;
    CEdit m_PChar;
    CEdit m_PWChar;
    CEdit m_Hex;
};
