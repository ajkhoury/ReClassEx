#ifndef SDK_H
#define SDK_H

#include "ReClass2016.h"

class ReclassSDK
{
public:
	virtual void Test()
	{
		MessageBox(0, _T("Plugin Loaded Successfully"), _T("ReclassSDK"), 0);
	}

	virtual void GetCurrentFilePath(LPTSTR szPath)
	{
		if (g_ReClassApp.CurrentFilePath.GetLength() > 0)
			_tcscpy(szPath, g_ReClassApp.CurrentFilePath.GetBuffer());
	}

	virtual size_t GetClassCount()
	{
		return g_ReClassApp.Classes.size();
	}

	virtual size_t GetNodeCount(unsigned int classId)
	{
		if (classId < g_ReClassApp.Classes.size())
		{
			CNodeBase* pBase = g_ReClassApp.Classes.at(classId);
			if (pBase)
				return pBase->NodeCount();
		}

		return -1;
	}

	virtual size_t CreateClass(LPTSTR szName)
	{
		// Try to find class
		int id = FindClassByName(szName);

		// Class already exists
		if (id != -1)
			return -1;

		// Create Class
		CNodeClass* pClass = new CNodeClass;
		pClass->SetName(szName);
		g_ReClassApp.Classes.push_back(pClass);

		// Open GUI Child Frame
		CMainFrame*  pFrame = STATIC_DOWNCAST(CMainFrame, AfxGetApp()->m_pMainWnd);
		CChildFrame* pChild = (CChildFrame*)pFrame->CreateNewChild(RUNTIME_CLASS(CChildFrame), IDR_ReClass2016TYPE, g_ReClassApp.m_hMDIMenu, g_ReClassApp.m_hMDIAccel);
		pChild->m_wndView.m_pClass = pClass;

		pClass->pChildWindow = pChild;
		g_ReClassApp.Classes.push_back(pClass);

		// Terrible; Fix this later
		return g_ReClassApp.Classes.size() - 1;

		//FindClassByName( szName );
		//FindClassByName( szName );
		// g_ReClassApp.Classes.back( )
		//
	}

	virtual int FindClassByName(LPTSTR szName, bool caseSensitive = true)
	{
		unsigned int id = 0;
		while (++id < g_ReClassApp.Classes.size())
		{
			if (caseSensitive)
			{
				if (_tcscmp(g_ReClassApp.Classes[id]->GetName(), szName) == 0)
					return id;
			}
			else
			{
				if (_tcsicmp(g_ReClassApp.Classes[id]->GetName(), szName) == 0)
					return id;
			}
		}
		return -1;
	}

	virtual bool AddNode(int classId, NodeType type, PCHAR szName)
	{
		CNodeBase* pBase = FindPointerByClassId(classId);
		if (!pBase)
		{
			PrintOut(_T("[!] Cannot find class %d"), classId);
			return false;
		}

		if (type != nt_vtable && type != nt_pointer)
		{
			CNodeBase* pNode = g_ReClassApp.CreateNewNode(type);
			// CNodeHex64* pNode = new CNodeHex64;
			pNode->SetParent(pBase);
			pNode->SetName(szName);

			pBase->AddNode(pNode);
			
			return TRUE;
		}

		return false;

	}

	__int16 major_version;
	__int16 minor_version;

	static ReclassSDK* m_pReclassSDK;

	static ReclassSDK* GetInstance()
	{
		if (m_pReclassSDK == NULL)
			m_pReclassSDK = new ReclassSDK();

		return m_pReclassSDK;
	}

private:
	int FindClassIdByPointer(CNodeBase* pBase)
	{
		unsigned int id = 0;

		while (++id < g_ReClassApp.Classes.size())
			if (g_ReClassApp.Classes.at(id)->GetParent() == pBase)
				return id;

		return -1;
	}

	CNodeBase* FindPointerByClassId(int Id)
	{
		if (Id < g_ReClassApp.Classes.size())
			return g_ReClassApp.Classes.at(Id);

		return NULL;
	}
};

__inline void LoadPlugin(LPTSTR pszPath)
{
	printf("Reclass SDK: %IX\n", (size_t)ReclassSDK::GetInstance());

	ReclassSDK::GetInstance()->major_version = 1;
	ReclassSDK::GetInstance()->minor_version = 1;

	HMODULE hPlugin = LoadLibrary(pszPath);

	printf("[?] Loaded plugin: %IX\n", (size_t)hPlugin);

	if (!hPlugin)
	{
		printf("[LoadPlugin!] GetLastError() = %d\n", GetLastError());
	}

	FARPROC pInit = GetProcAddress(hPlugin, "Init");
	printf("[?] Init function = %IX\n", (size_t)pInit);

	if (pInit)
	{
		typedef void(__cdecl* tInit)(ReclassSDK* pReclassSDK);
		tInit Init = (tInit)pInit;
		Init(ReclassSDK::GetInstance());
	}
}

#endif SDK_H