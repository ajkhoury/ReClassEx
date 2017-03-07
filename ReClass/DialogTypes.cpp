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
	DDX_Control( pDX, IDC_EDIT_QWORD, m_QWord );
	DDX_Control( pDX, IDC_EDIT_DWORD, m_DWord );
	DDX_Control( pDX, IDC_EDIT_WORD, m_Word );
	DDX_Control( pDX, IDC_EDIT_BYTE, m_BYTE );
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

	m_Hex.SetWindowText( g_tdHex );

	m_Int64.SetWindowText( g_tdInt64 );
	m_Int32.SetWindowText( g_tdInt32 );
	m_Int16.SetWindowText( g_tdInt16 );
	m_Int8.SetWindowText( g_tdInt8 );

	m_QWord.SetWindowText( g_tdQWORD );
	m_DWord.SetWindowText( g_tdDWORD );
	m_Word.SetWindowText( g_tdWORD );
	m_BYTE.SetWindowText( g_tdBYTE );

	m_Vec2.SetWindowText( g_tdVec2 );
	m_Vec3.SetWindowText( g_tdVec3 );
	m_Quat.SetWindowText( g_tdQuat );
	m_Matrix.SetWindowText( g_tdMatrix );
	m_PChar.SetWindowText( g_tdPChar );
	m_PWChar.SetWindowText( g_tdPWChar );

	return TRUE;
}

void CDialogTypes::OnOK( )
{
	m_Hex.GetWindowText(	g_tdHex );

	m_Int64.GetWindowText(	g_tdInt64 );
	m_Int32.GetWindowText(	g_tdInt32 );
	m_Int16.GetWindowText(	g_tdInt16 );
	m_Int8.GetWindowText(	g_tdInt8 );

	m_QWord.GetWindowText(	g_tdQWORD );
	m_DWord.GetWindowText(	g_tdDWORD );
	m_Word.GetWindowText(	g_tdWORD );
	m_BYTE.GetWindowText(	g_tdBYTE );

	m_Vec2.GetWindowText(	g_tdVec2 );
	m_Vec3.GetWindowText(	g_tdVec3 );
	m_Quat.GetWindowText(	g_tdQuat );
	m_Matrix.GetWindowText( g_tdMatrix );
	m_PChar.GetWindowText(	g_tdPChar );
	m_PWChar.GetWindowText( g_tdPWChar );

	CDialogEx::OnOK( );
}
