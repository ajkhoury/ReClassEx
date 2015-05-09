// DialogTypes.cpp : implementation file
//
#include "stdafx.h"
#include "ReClass2015.h"
#include "DialogTypes.h"
#include "afxdialogex.h"


// CDialogTypes dialog

IMPLEMENT_DYNAMIC(CDialogTypes, CDialogEx)

CDialogTypes::CDialogTypes(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDialogTypes::IDD, pParent)
{

}

CDialogTypes::~CDialogTypes()
{
}

void CDialogTypes::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_INT64, m_Int64);
	DDX_Control(pDX, IDC_EDIT_INT32, m_Int32);
	DDX_Control(pDX, IDC_EDIT_INT16, m_Int16);
	DDX_Control(pDX, IDC_EDIT_INT8, m_Int8);
	DDX_Control(pDX, IDC_EDIT_DWORD, m_DWord);
	DDX_Control(pDX, IDC_EDIT_WORD, m_Word);
	DDX_Control(pDX, IDC_EDIT_BYTE, m_BYTE);
	DDX_Control(pDX, IDC_EDIT_VEC2, m_Vec2);
	DDX_Control(pDX, IDC_EDIT_VEC3, m_Vec3);
	DDX_Control(pDX, IDC_EDIT_QUAT, m_Quat);
	DDX_Control(pDX, IDC_EDIT_MATRIX, m_Matrix);
	DDX_Control(pDX, IDC_EDIT_HEX, m_Hex);
	DDX_Control(pDX, IDC_EDIT_PCHAR, m_PChar);
}


BEGIN_MESSAGE_MAP(CDialogTypes, CDialogEx)
END_MESSAGE_MAP()


// CDialogTypes message handlers
BOOL CDialogTypes::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_Hex.SetWindowText(tdHex);

	m_Int64.SetWindowText(tdInt64);
	m_Int32.SetWindowText(tdInt32);
	m_Int16.SetWindowText(tdInt16);
	m_Int8.SetWindowText(tdInt8);

	m_DWord.SetWindowText(tdDWORD);
	m_Word.SetWindowText(tdWORD);
	m_BYTE.SetWindowText(tdBYTE);

	m_Vec2.SetWindowText(tdVec2);
	m_Vec3.SetWindowText(tdVec3);
	m_Quat.SetWindowText(tdQuat);
	m_Matrix.SetWindowText(tdMatrix);
	m_PChar.SetWindowText(tdPChar);

	return TRUE;
}


void CDialogTypes::OnOK()
{
	m_Hex.GetWindowText(tdHex);

	m_Int64.GetWindowText(tdInt64);
	m_Int32.GetWindowText(tdInt32);
	m_Int16.GetWindowText(tdInt16);
	m_Int8.GetWindowText(tdInt8);

	m_DWord.GetWindowText(tdDWORD);
	m_Word.GetWindowText(tdWORD);
	m_BYTE.GetWindowText(tdBYTE);

	m_Vec2.GetWindowText(tdVec2);
	m_Vec3.GetWindowText(tdVec3);
	m_Quat.GetWindowText(tdQuat);
	m_Matrix.GetWindowText(tdMatrix);
	m_PChar.GetWindowText(tdPChar);

	CDialogEx::OnOK();
}
