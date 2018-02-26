// DialogTypes.cpp : implementation file
//
#include "stdafx.h"
#include "ReClassEx.h"
#include "DialogTypes.h"
#include "afxdialogex.h"


// CDialogTypes dialog

IMPLEMENT_DYNAMIC( CDialogTypes, CDialogEx )

CDialogTypes::CDialogTypes( CWnd* pParent /*=NULL*/ )
    : CDialogEx( CDialogTypes::IDD, pParent )
{
}

CDialogTypes::~CDialogTypes( )
{
}

void CDialogTypes::DoDataExchange( CDataExchange* pDX )
{
    CDialogEx::DoDataExchange( pDX );
    DDX_Control( pDX, IDC_EDIT_INT64, m_Int64 );
    DDX_Control( pDX, IDC_EDIT_INT32, m_Int32 );
    DDX_Control( pDX, IDC_EDIT_INT16, m_Int16 );
    DDX_Control( pDX, IDC_EDIT_INT8, m_Int8 );
    DDX_Control( pDX, IDC_EDIT_QWORD, m_Qword );
    DDX_Control( pDX, IDC_EDIT_DWORD, m_Dword );
    DDX_Control( pDX, IDC_EDIT_WORD, m_Word );
    DDX_Control( pDX, IDC_EDIT_BYTE, m_Byte );
    DDX_Control( pDX, IDC_EDIT_VEC2, m_Vec2 );
    DDX_Control( pDX, IDC_EDIT_VEC3, m_Vec3 );
    DDX_Control( pDX, IDC_EDIT_QUAT, m_Quat );
    DDX_Control( pDX, IDC_EDIT_MATRIX, m_Matrix );
    DDX_Control( pDX, IDC_EDIT_HEX, m_Hex );
    DDX_Control( pDX, IDC_EDIT_PCHAR, m_PChar );
    DDX_Control( pDX, IDC_EDIT_PWCHAR, m_PWChar );
}


BEGIN_MESSAGE_MAP( CDialogTypes, CDialogEx )
END_MESSAGE_MAP( )


// CDialogTypes message handlers
BOOL CDialogTypes::OnInitDialog( )
{
    CDialogEx::OnInitDialog( );

    m_Hex.SetWindowText( g_Typedefs.Hex );

    m_Int64.SetWindowText( g_Typedefs.Int64 );
    m_Int32.SetWindowText( g_Typedefs.Int32 );
    m_Int16.SetWindowText( g_Typedefs.Int16 );
    m_Int8.SetWindowText( g_Typedefs.Int8 );

    m_Qword.SetWindowText( g_Typedefs.Qword );
    m_Dword.SetWindowText( g_Typedefs.Dword );
    m_Word.SetWindowText( g_Typedefs.Word );
    m_Byte.SetWindowText( g_Typedefs.Byte );

    m_Vec2.SetWindowText( g_Typedefs.Vec2 );
    m_Vec3.SetWindowText( g_Typedefs.Vec3 );
    m_Quat.SetWindowText( g_Typedefs.Quat );
    m_Matrix.SetWindowText( g_Typedefs.Matrix );
    m_PChar.SetWindowText( g_Typedefs.PChar );
    m_PWChar.SetWindowText( g_Typedefs.PWChar );

    return TRUE;
}

void CDialogTypes::OnOK( )
{
    m_Hex.GetWindowText( g_Typedefs.Hex );

    m_Int64.GetWindowText( g_Typedefs.Int64 );
    m_Int32.GetWindowText( g_Typedefs.Int32 );
    m_Int16.GetWindowText( g_Typedefs.Int16 );
    m_Int8.GetWindowText( g_Typedefs.Int8 );

    m_Qword.GetWindowText( g_Typedefs.Qword );
    m_Dword.GetWindowText( g_Typedefs.Dword );
    m_Word.GetWindowText( g_Typedefs.Word );
    m_Byte.GetWindowText( g_Typedefs.Byte );

    m_Vec2.GetWindowText( g_Typedefs.Vec2 );
    m_Vec3.GetWindowText( g_Typedefs.Vec3 );
    m_Quat.GetWindowText( g_Typedefs.Quat );
    m_Matrix.GetWindowText( g_Typedefs.Matrix );
    m_PChar.GetWindowText( g_Typedefs.PChar );
    m_PWChar.GetWindowText( g_Typedefs.PWChar );

    CDialogEx::OnOK( );
}
