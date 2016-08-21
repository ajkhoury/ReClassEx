#include "stdafx.h"
#include "SymbolReader.h"

//#include <direct.h>
//#define GetCurrentDir getcwd

SymbolReader::SymbolReader() : 
	m_bInitialized(false), 
	m_pSource(0), 
	m_pSession(0), 
	m_pGlobal(0)
{
}

SymbolReader::~SymbolReader()
{
	if (m_pGlobal)
		m_pGlobal->Release();
	if (m_pSession)
		m_pSession->Release();
	if (m_pSource)
		m_pSource->Release();
}

bool SymbolReader::LoadSymbolData(const TCHAR* pszSearchPath)
{
	TCHAR szExt[MAX_PATH];
	DWORD dwMachType = 0;
	HRESULT hr = S_OK;

	// Obtain access to the provider
	hr = CoCreateInstance(__uuidof(DiaSource), NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&m_pSource));
	if (FAILED(hr))
	{
		PrintOut(_T("[LoadDataFromPdb] CoCreateInstance failed - HRESULT = %08X"), hr);
		return false;
	}

	_tsplitpath_s(m_strFilePath.GetString(), NULL, 0, NULL, 0, NULL, 0, szExt, MAX_PATH);
	if (!_tcsicmp(szExt, _T(".pdb"))) 
	{
		// Open and prepare a program database (.pdb) file as a debug data source
		hr = m_pSource->loadDataFromPdb(m_strFilePath.GetString());
		if (FAILED(hr))
		{
			_com_error err(hr);
			LPCTSTR errMsg = err.ErrorMessage();
			PrintOut(_T("[LoadDataFromPdb] loadDataFromPdb failed - %s"), errMsg);
			return false;
		}
	}
	else 
	{
		// Open and prepare the debug data associated with the executable
		hr = m_pSource->loadDataForExe(m_strFilePath.GetString(), pszSearchPath, NULL);
		if (FAILED(hr))
		{
			LPCTSTR errMsg = 0;
			switch (hr)
			{
			case E_PDB_NOT_FOUND:
				errMsg = _T("PDB not found");
				break;
			case E_PDB_FORMAT:
				errMsg = _T("Invalid PDB format");
				break;
			case E_PDB_INVALID_SIG:
				errMsg = _T("Invalid PDB signature");
				break;
			case E_PDB_INVALID_AGE:
				errMsg = _T("Invalid PDB age");
				break;
			case E_PDB_NO_DEBUG_INFO:
				errMsg = _T("No debug info found in PDB");
				break;
			default:
				_com_error err(hr);
				errMsg = err.ErrorMessage();
				break;
			}

			PrintOut(_T("[LoadDataFromPdb] loadDataForExe failed - %s"), errMsg);
			return false;
		}
	}

	// Open a session for querying symbols
	hr = m_pSource->openSession(&m_pSession);
	if (FAILED(hr)) 
	{
		PrintOut(_T("[LoadDataFromPdb] openSession failed - HRESULT = %08X"), hr);
		return false;
	}

	// Retrieve a reference to the global scope
	hr = m_pSession->get_globalScope(&m_pGlobal);
	if (FAILED(hr))
	{
		PrintOut(_T("[LoadDataFromPdb] get_globalScope failed - HRESULT = %08X"), hr);
		return false;
	}

	// Set Machine type for getting correct register names
	if (m_pGlobal->get_machineType(&dwMachType) == S_OK)
	{
		switch (dwMachType)
		{
		case IMAGE_FILE_MACHINE_I386: m_dwMachineType = CV_CFL_80386; break;
		case IMAGE_FILE_MACHINE_IA64: m_dwMachineType = CV_CFL_IA64; break;
		case IMAGE_FILE_MACHINE_AMD64: m_dwMachineType = CV_CFL_AMD64; break;
		}
	}

	return true;
}

////////////////////////////////////////////////////////////
// Print the string coresponding to the symbol's tag property
//
void SymbolReader::ReadSymTag(DWORD dwSymTag, CString& outString)
{
	CString append;
	append.Format(_T("%s: "), SafeDRef(rgTags, dwSymTag));
	outString += append;
	//wprintf(L"%-15s: ", SafeDRef(rgTags, dwSymTag));
}

////////////////////////////////////////////////////////////
// Print the name of the symbol
//
void SymbolReader::ReadName(IDiaSymbol *pSymbol, CString& outString)
{
	BSTR bstrName;
	BSTR bstrUndName;

	if (pSymbol->get_name(&bstrName) != S_OK) 
	{
		//outString += _T("(none)");
		//wprintf(L"(none)");
		return;
	}

	if (pSymbol->get_undecoratedName(&bstrUndName) == S_OK) 
	{
		if (wcscmp(bstrName, bstrUndName) == 0) 
		{
			outString += bstrName;
			//wprintf(L"%s", bstrName);
		}
		else 
		{
			CString append;
			append.Format(_T("%s(%s)"), bstrUndName, bstrName);
			outString += append;
			//wprintf(L"%s(%s)", bstrUndName, bstrName);
		}

		SysFreeString(bstrUndName);
	}
	else 
	{
		outString += bstrName;
		//wprintf(L"%s", bstrName);
	}

	SysFreeString(bstrName);
}

////////////////////////////////////////////////////////////
// Print a VARIANT
//
void SymbolReader::ReadVariant(VARIANT var, CString& outString)
{
	CString append;

	switch (var.vt)
	{
	case VT_UI1:
	case VT_I1:
	{
		append.Format(_T(" 0x%X"), var.bVal);
		outString += append;
		//wprintf(L" 0x%X", var.bVal);
	}
	break;

	case VT_I2:
	case VT_UI2:
	case VT_BOOL:
	{
		append.Format(_T(" 0x%X"), var.iVal);
		outString += append;
		//wprintf(L" 0x%X", var.iVal);
	}
	break;

	case VT_I4:
	case VT_UI4:
	case VT_INT:
	case VT_UINT:
	case VT_ERROR:
	{
		append.Format(_T(" 0x%X"), var.lVal);
		outString += append;
		//wprintf(L" 0x%X", var.lVal);
	}
	break;

	case VT_R4:
	{
		append.Format(_T(" %g"), var.fltVal);
		outString += append;
		//wprintf(L" %g", var.fltVal);
	}
	break;

	case VT_R8:
	{
		append.Format(_T(" %g"), var.dblVal);
		outString += append;
		//wprintf(L" %g", var.dblVal);
	}
	break;

	case VT_BSTR:
	{
		append.Format(_T(" \"%s\""), var.bstrVal);
		outString += append;
		//wprintf(L" \"%s\"", var.bstrVal);
	}
	break;

	default:
	{
		outString += _T(" ??");
		//wprintf(L" ??");
	}
	break;

	}
}

void SymbolReader::ReadBound(IDiaSymbol *pSymbol, CString& outString)
{
	DWORD dwTag = 0;
	DWORD dwKind;
	HRESULT hr = S_OK;

	hr = pSymbol->get_symTag(&dwTag);
	if (FAILED(hr)) 
	{
		PrintOut(_T("[ReadBound] ERROR - get_symTag failed - HRESULT %08X"), hr);
		return;
	}

	hr = pSymbol->get_locationType(&dwKind);
	if (FAILED(hr))
	{
		PrintOut(_T("[ReadBound] ERROR - get_locationType - HRESULT %08X"), hr);
		return;
	}

	if (dwTag == SymTagData && dwKind == LocIsConstant) 
	{
		VARIANT v;
		if (pSymbol->get_value(&v) == S_OK) 
		{
			ReadVariant(v, outString);
			VariantClear((VARIANTARG *)&v);
		}
	}
	else 
	{
		ReadName(pSymbol, outString);
	}
}

////////////////////////////////////////////////////////////
// Print a string corespondig to a location type
//
void SymbolReader::ReadLocation(IDiaSymbol *pSymbol, CString& outString)
{
	DWORD dwLocType;
	DWORD dwRVA, dwSect, dwOff, dwReg, dwBitPos, dwSlot;
	LONG lOffset;
	ULONGLONG ulLen;
	VARIANT vt = { VT_EMPTY };

	if (pSymbol->get_locationType(&dwLocType) != S_OK) 
	{
		// It must be a symbol in optimized code
		PrintOut(_T("[ReadLocation] Symbol in optimized code!"));
		return;
	}

	CString append;
	switch (dwLocType)
	{
	case LocIsStatic:
	{
		if ((pSymbol->get_relativeVirtualAddress(&dwRVA) == S_OK) &&
			(pSymbol->get_addressSection(&dwSect) == S_OK) &&
			(pSymbol->get_addressOffset(&dwOff) == S_OK))
		{
			append.Format(_T("%s, [%08X][%04X:%08X]"), SafeDRef(rgLocationTypeString, dwLocType), dwRVA, dwSect, dwOff);
			outString += append;
			//wprintf(L"%s, [%08X][%04X:%08X]", SafeDRef(rgLocationTypeString, dwLocType), dwRVA, dwSect, dwOff);
		}
	}
	break;

	case LocIsTLS:
	case LocInMetaData:
	case LocIsIlRel:
	{
		if ((pSymbol->get_relativeVirtualAddress(&dwRVA) == S_OK) &&
			(pSymbol->get_addressSection(&dwSect) == S_OK) &&
			(pSymbol->get_addressOffset(&dwOff) == S_OK))
		{
			append.Format(_T("%s, [%08X][% 04X:%08X]"), SafeDRef(rgLocationTypeString, dwLocType), dwRVA, dwSect, dwOff);
			outString += append;
			//wprintf(L"%s, [%08X][%04X:%08X]", SafeDRef(rgLocationTypeString, dwLocType), dwRVA, dwSect, dwOff);
		}
	}
	break;

	case LocIsRegRel:
	{
		if ((pSymbol->get_registerId(&dwReg) == S_OK) &&
			(pSymbol->get_offset(&lOffset) == S_OK))
		{
			append.Format(_T("Relative, [%08X]"), lOffset);
			outString += append;
			//wprintf(L"Relative, [%08X]", lOffset);
		}
	}
	break;

	case LocIsThisRel:
	{
		if (pSymbol->get_offset(&lOffset) == S_OK)
		{
			append.Format(_T("this+0x%X"), lOffset);
			outString += append;
			//wprintf(L"this+0x%X", lOffset);
		}
	}
	break;

	case LocIsBitField:
	{
		if ((pSymbol->get_offset(&lOffset) == S_OK) &&
			(pSymbol->get_bitPosition(&dwBitPos) == S_OK) &&
			(pSymbol->get_length(&ulLen) == S_OK))
		{
			append.Format(_T("this(bf)+0x%X:0x%X len(0x%X)"), lOffset, dwBitPos, (unsigned int)ulLen);
			outString += append;
			//wprintf(L"this(bf)+0x%X:0x%X len(0x%X)", lOffset, dwBitPos, (unsigned int)ulLen);
		}
	}
	break;

	case LocIsEnregistered:
	{
		if (pSymbol->get_registerId(&dwReg) == S_OK)
		{
			append.Format(_T("enregistered "));
			outString += append;
			//wprintf(L"enregistered ");
		}
	}
	break;

	case LocIsSlot:
	{
		if (pSymbol->get_slot(&dwSlot) == S_OK)
		{
			append.Format(_T("%s, [%08X]"), SafeDRef(rgLocationTypeString, dwLocType), dwSlot);
			outString += append;
			//wprintf(L"%s, [%08X]", SafeDRef(rgLocationTypeString, dwLocType), dwSlot);
		}
	}
	break;

	case LocIsConstant:
	{
		outString += _T("constant");
		//wprintf(L"constant");
		if (pSymbol->get_value(&vt) == S_OK)
		{
			ReadVariant(vt, outString);
			VariantClear((VARIANTARG *)&vt);
		}
	}
	break;

	case LocIsNull:
		break;

	default:
		PrintOut(_T("Error - invalid location type: %d"), dwLocType);
		break;
	}
}

////////////////////////////////////////////////////////////
// Print a string corresponding to a UDT kind
//
void SymbolReader::ReadUdtKind(IDiaSymbol *pSymbol, CString& outString)
{
	DWORD dwKind = 0;
	if (pSymbol->get_udtKind(&dwKind) == S_OK) 
	{
		outString += rgUdtKind[dwKind];
		outString += _T(' ');
		//wprintf(L"%s ", rgUdtKind[dwKind]);
	}
}

////////////////////////////////////////////////////////////
// Print the information details for a type symbol
//
void SymbolReader::ReadType(IDiaSymbol *pSymbol, CString& outString)
{
	IDiaSymbol *pBaseType;
	IDiaEnumSymbols *pEnumSym;
	IDiaSymbol *pSym;
	DWORD dwTag;
	BSTR bstrName;
	DWORD dwInfo;
	BOOL bSet;
	DWORD dwRank;
	LONG lCount = 0;
	ULONG celt = 1;

	if (pSymbol->get_symTag(&dwTag) != S_OK)
	{
		PrintOut(_T("[ReadType] ERROR - cannot retrieve the symbol's SymTag"));
		return;
	}

	if (pSymbol->get_name(&bstrName) != S_OK)
		bstrName = NULL;

	//wprintf(L"bstrName: %s\n", bstrName);

	if (dwTag != SymTagPointerType)
	{
		if ((pSymbol->get_constType(&bSet) == S_OK) && bSet)
		{
			outString += _T("const ");
			//wprintf(L"const ");
		}
		if ((pSymbol->get_volatileType(&bSet) == S_OK) && bSet)
		{
			outString += _T("volatile ");
			//wprintf(L"volatile ");
		}
		if ((pSymbol->get_unalignedType(&bSet) == S_OK) && bSet)
		{
			outString += _T("__unaligned ");
			//wprintf(L"__unaligned ");
		}
	}

	ULONGLONG ulLen;
	pSymbol->get_length(&ulLen);

	switch (dwTag)
	{
	case SymTagUDT:
		ReadUdtKind(pSymbol, outString);
		ReadName(pSymbol, outString);
		break;

	case SymTagEnum:
		outString += _T("enum ");
		//wprintf(L"enum ");
		ReadName(pSymbol, outString);
		break;

	case SymTagFunctionType:
		outString += _T("function ");
		//wprintf(L"function ");
		break;

	case SymTagPointerType:
		if (pSymbol->get_type(&pBaseType) != S_OK)
		{
			PrintOut(_T("[ReadType] ERROR - SymTagPointerType get_type"));
			if (bstrName != NULL)
				SysFreeString(bstrName);
			return;
		}

		ReadType(pBaseType, outString);
		pBaseType->Release();

		if ((pSymbol->get_reference(&bSet) == S_OK) && bSet) 
		{
			outString += _T(" &");
			//wprintf(L" &");
		}
		else 
		{
			outString += _T(" *");
			//wprintf(L" *");
		}

		if ((pSymbol->get_constType(&bSet) == S_OK) && bSet)
		{
			outString += _T(" const");
			//wprintf(L" const");
		}
		if ((pSymbol->get_volatileType(&bSet) == S_OK) && bSet)
		{
			outString += _T(" volatile");
			//wprintf(L" volatile");
		}
		if ((pSymbol->get_unalignedType(&bSet) == S_OK) && bSet)
		{
			outString += _T(" __unaligned");
			//wprintf(L" __unaligned");
		}

		break;

	case SymTagArrayType:
	{
		if (pSymbol->get_type(&pBaseType) == S_OK)
		{
			ReadType(pBaseType, outString);
			if (pSymbol->get_rank(&dwRank) == S_OK)
			{
				if (SUCCEEDED(pSymbol->findChildren(SymTagDimension, NULL, nsNone, &pEnumSym)) && (pEnumSym != NULL))
				{
					while (SUCCEEDED(pEnumSym->Next(1, &pSym, &celt)) && (celt == 1))
					{
						outString += _T("[");

						IDiaSymbol *pBound;
						if (pSym->get_lowerBound(&pBound) == S_OK)
						{
							ReadBound(pBound, outString);
							outString += _T("..");
							pBound->Release();
						}

						pBound = NULL;
						if (pSym->get_upperBound(&pBound) == S_OK)
						{
							ReadBound(pBound, outString);
							pBound->Release();
						}

						pSym->Release();
						pSym = NULL;

						outString += _T("]");
					}

					pEnumSym->Release();
				}
			}
			else if (SUCCEEDED(pSymbol->findChildren(SymTagCustomType, NULL, nsNone, &pEnumSym)) &&
				(pEnumSym != NULL) && (pEnumSym->get_Count(&lCount) == S_OK) && (lCount > 0))
			{
				while (SUCCEEDED(pEnumSym->Next(1, &pSym, &celt)) && (celt == 1))
				{
					outString += _T("[");
					ReadType(pSym, outString);
					outString += _T("]");

					pSym->Release();
				}

				pEnumSym->Release();
			}
			else
			{
				DWORD dwCountElems;
				ULONGLONG ulLenArray;
				ULONGLONG ulLenElem;

				if (pSymbol->get_count(&dwCountElems) == S_OK)
				{
					CString append;
					append.Format(_T("[%d]"), dwCountElems);
					outString += append;
					//wprintf(L"[%d]", dwCountElems);
				}
				else if ((pSymbol->get_length(&ulLenArray) == S_OK) && (pBaseType->get_length(&ulLenElem) == S_OK))
				{
					CString append;
					if (ulLenElem == 0)
					{
						append.Format(_T("[%d]"), ulLenArray);
						outString += append;
						//wprintf(L"[%d]", ulLenArray);
					}
					else
					{
						append.Format(_T("[%d]"), ulLenArray / ulLenElem);
						outString += append;
						//wprintf(L"[%d]", ulLenArray / ulLenElem);
					}
				}
			}

			pBaseType->Release();
		}
		else
		{
			PrintOut(_T("ERROR - SymTagArrayType get_type"));
			if (bstrName != NULL)
				SysFreeString(bstrName);
			return;
		}
	}
	break;

	case SymTagBaseType:
	{
		if (pSymbol->get_baseType(&dwInfo) != S_OK)
		{
			PrintOut(_T("SymTagBaseType get_baseType"));
			if (bstrName != NULL)
				SysFreeString(bstrName);
			return;
		}

		switch (dwInfo)
		{
		case btUInt:
		{
			outString += _T("unsigned ");
			//wprintf(L"unsigned ");	
		} // Fall through
		case btInt:
		{
			switch (ulLen)
			{
			case 1:
			{
				if (dwInfo == btInt)
				{
					outString += _T("signed ");
					//wprintf(L"signed ");
				}
				outString += _T("char");
				//wprintf(L"char");		
			}
			break;

			case 2:
			{
				outString += _T("short");
				//wprintf(L"short");
			}
			break;

			case 4:
			{
				outString += _T("int");
				//wprintf(L"int");
			}
			break;

			case 8:
			{
				outString += _T("__int64");
				//wprintf(L"__int64");
			}
			break;

			}

			dwInfo = 0xFFFFFFFF;
		}
		break;

		case btFloat:
		{
			switch (ulLen)
			{
			case 4:
			{
				outString += _T("float");
				//wprintf(L"float");
			}
			break;

			case 8:
			{
				outString += _T("double");
				//wprintf(L"double");
			}
			break;

			}

			dwInfo = 0xFFFFFFFF;
		}
		break;

		}

		if (dwInfo == 0xFFFFFFFF)
			break;

		outString += rgBaseType[dwInfo];
		//wprintf(L"%s", rgBaseType[dwInfo]);
	}
	break;

	case SymTagTypedef:
	{
		ReadName(pSymbol, outString);
	}
	break;

	case SymTagCustomType:
	{
		DWORD cbData = 0;
		DWORD count;

		if (pSymbol->get_types(0, &count, NULL) == S_OK)
		{
			IDiaSymbol** rgpDiaSymbols = (IDiaSymbol**)_alloca(sizeof(IDiaSymbol *)* count);
			if (pSymbol->get_types(count, &count, rgpDiaSymbols) == S_OK)
			{
				for (ULONG i = 0; i < count; i++)
				{
					ReadType(rgpDiaSymbols[i], outString);
					rgpDiaSymbols[i]->Release();
				}
			}
		}

		// print custom data
		if ((pSymbol->get_dataBytes(cbData, &cbData, NULL) == S_OK) && (cbData != 0))
		{
			outString += _T(" data: ");
			//wprintf(L", Data: ");

			BYTE *pbData = new BYTE[cbData];
			pSymbol->get_dataBytes(cbData, &cbData, pbData);

			for (ULONG i = 0; i < cbData; i++)
			{
				CString append;
				append.Format(_T("%02X "), pbData[i]);
				outString += append;
				//wprintf(L"0x%02X ", pbData[i]);
			}

			delete[] pbData;
		}
	}
	break;

	case SymTagData: // This really is member data, just print its location
		ReadLocation(pSymbol, outString);
		break;
	}

	if (bstrName != NULL)
		SysFreeString(bstrName);
}

////////////////////////////////////////////////////////////
// Print a string representing the type of a symbol
//
void SymbolReader::ReadSymbolType(IDiaSymbol *pSymbol, CString& outString)
{
	IDiaSymbol *pType;
	if (pSymbol->get_type(&pType) == S_OK) 
	{
		//wprintf(L" Type: ");
		ReadType(pType, outString);
		pType->Release();
		outString += _T(' ');
	}
}

////////////////////////////////////////////////////////////
//
void SymbolReader::ReadData(IDiaSymbol *pSymbol, CString& outString)
{
	ReadLocation(pSymbol, outString);

	HRESULT hr = S_OK;
	DWORD dwDataKind;

	hr = pSymbol->get_dataKind(&dwDataKind);
	if (FAILED(hr))
	{
		PrintOut(_T("[ReadData] ERROR - get_dataKind - %08X"), hr);
		return;
	}

	outString += _T(' ');
	outString += SafeDRef(rgDataKind, dwDataKind);
	outString += _T(' ');
	//wprintf(L", %s", SafeDRef(rgDataKind, dwDataKind));
	ReadSymbolType(pSymbol, outString);

	//outString += _T(", ");
	//wprintf(L", ");
	//ReadName(pSymbol, outString);
}

////////////////////////////////////////////////////////////
// Print the undecorated name of the symbol
//  - only SymTagFunction, SymTagData and SymTagPublicSymbol
//    can have this property set
//
void SymbolReader::ReadUndName(IDiaSymbol *pSymbol, CString& outString)
{
	BSTR bstrName;
	if (pSymbol->get_undecoratedName(&bstrName) != S_OK) 
	{
		if (pSymbol->get_name(&bstrName) == S_OK) 
		{
			// Print the name of the symbol instead
			outString += (bstrName[0] != _T('\0')) ? bstrName : _T("");
			//wprintf(L"%s", (bstrName[0] != L'\0') ? bstrName : L"(none)");
			SysFreeString(bstrName);
		}
		else 
		{
			//outString += _T("(none)");
			//wprintf(L"(none)");
		}

		return;
	}

	if (bstrName[0] != _T('\0')) {
		outString += bstrName;
		//wprintf(L"%s", bstrName);
	}

	SysFreeString(bstrName);
}

////////////////////////////////////////////////////////////
// Print the name and the type of an user defined type
//
void SymbolReader::ReadUDT(IDiaSymbol *pSymbol, CString& outString)
{
	ReadSymbolType(pSymbol, outString);
	ReadName(pSymbol, outString);
}

////////////////////////////////////////////////////////////
// Print a symbol info: name, type etc.
//
void SymbolReader::ReadSymbol(IDiaSymbol *pSymbol, CString& outString)
{
	IDiaSymbol *pType;
	DWORD dwSymTag;
	ULONGLONG ulLen;
	HRESULT hr = S_OK;
	
	hr = pSymbol->get_symTag(&dwSymTag);
	if (FAILED(hr))
	{
		PrintOut(_T("[ReadSymbol] ERROR - PrintSymbol get_symTag() failed - HRESULT %08X"), hr);
		return;
	}

	//ReadSymTag(dwSymTag, outString);

	switch (dwSymTag) 
	{
	case SymTagCompilandDetails:
		//PrintCompilandDetails(pSymbol);
		break;

	case SymTagCompilandEnv:
		//PrintCompilandEnv(pSymbol);
		break;

	case SymTagData:
		ReadData(pSymbol, outString);
		break;

	case SymTagFunction:
	case SymTagBlock:
	{
		//ReadLocation(pSymbol, outString);
		if (dwSymTag != SymTagFunction && pSymbol->get_length(&ulLen) == S_OK)
		{
			CString append;
			append.Format(_T(" len(%08X) "), (ULONG)ulLen);
			outString += append;
			//wprintf(L", len = %08X, ", (ULONG)ulLen);
		}

		if (dwSymTag == SymTagFunction)
		{
			DWORD dwCall;
			if (pSymbol->get_callingConvention(&dwCall) == S_OK)
			{
				CString append;
				append.Format(_T(" %s"), SafeDRef(rgCallingConvention, dwCall));
				outString += append;
				//wprintf(L", %s", SafeDRef(rgCallingConvention, dwCall));
			}
		}

		ReadName(pSymbol, outString);

		IDiaEnumSymbols *pEnumChildren;
		if (SUCCEEDED(pSymbol->findChildren(SymTagNull, NULL, nsNone, &pEnumChildren)))
		{
			IDiaSymbol *pChild;
			ULONG celt = 0;
			while (SUCCEEDED(pEnumChildren->Next(1, &pChild, &celt)) && (celt == 1))
			{
				ReadSymbol(pChild, outString);
				pChild->Release();
			}	
			pEnumChildren->Release();
		}
	}
	return;

	case SymTagAnnotation:
		//ReadLocation(pSymbol, outString);
		//putwchar(L'\n');
		break;

	case SymTagLabel:
		ReadLocation(pSymbol, outString);
		//outString += _T(", ");
		//wprintf(L", ");
		ReadName(pSymbol, outString);
		break;

	case SymTagEnum:
	case SymTagTypedef:
	case SymTagUDT:
	case SymTagBaseClass:
		ReadUDT(pSymbol, outString);
		break;

	case SymTagFuncDebugStart:
	case SymTagFuncDebugEnd:
		//ReadLocation(pSymbol, outString);
		break;

	case SymTagFunctionArgType:
	case SymTagFunctionType:
	case SymTagPointerType:
	case SymTagArrayType:
	case SymTagBaseType:
		if (pSymbol->get_type(&pType) == S_OK) 
		{
			ReadType(pType, outString);
			pType->Release();
		}
		//putwchar(L'\n');
		break;

	case SymTagThunk:
		//PrintThunk(pSymbol);
		break;

	case SymTagCallSite:
		//PrintCallSiteInfo(pSymbol);
		break;

	case SymTagHeapAllocationSite:
		//PrintHeapAllocSite(pSymbol);
		break;

	case SymTagCoffGroup:
		//PrintCoffGroup(pSymbol);
		break;

	default:
		ReadSymbolType(pSymbol, outString);
		ReadName(pSymbol, outString);

		//if (pSymbol->get_type(&pType) == S_OK)
		//{
		//	outString += _T(" type ");
		//	//wprintf(L" has type ");
		//	ReadType(pType, outString);
		//	pType->Release();
		//}
	}

	if ((dwSymTag == SymTagUDT) || (dwSymTag == SymTagAnnotation))
	{
		IDiaEnumSymbols *pEnumChildren;

		//putwchar(L'\n');

		if (SUCCEEDED(pSymbol->findChildren(SymTagNull, NULL, nsNone, &pEnumChildren))) 
		{
			IDiaSymbol *pChild;
			ULONG celt = 0;

			while (SUCCEEDED(pEnumChildren->Next(1, &pChild, &celt)) && (celt == 1)) 
			{
				ReadSymbol(pChild, outString);
				pChild->Release();
			}

			pEnumChildren->Release();
		}
	}
	//putwchar(L'\n');
}


bool SymbolReader::GetSymbolStringWithVA(size_t dwVA, CString& outString)
{
	IDiaSymbol *pSymbol;
	LONG lDisplacement;

	//#ifdef _DEBUG
	//if (dwVA == 0x7FF6AB662DE8)
	//	PrintOut(_T("Test!"));
	//#endif
	
	size_t dwRVA = dwVA - m_dwModuleBase;

	if (FAILED(m_pSession->findSymbolByRVAEx((DWORD)dwRVA, SymTagNull, &pSymbol, &lDisplacement))) 
		return false;

	ReadSymbol(pSymbol, outString);
	pSymbol->Release();

	return true;
}

bool SymbolReader::LoadFile(CString FilePath, size_t dwBaseAddr, DWORD dwModuleSize, const TCHAR* pszSearchPath)
{
	int idx = FilePath.ReverseFind('/');
	if (idx == -1)
		idx = FilePath.ReverseFind('\\');
	return LoadFile(FilePath.Mid(++idx), FilePath, dwBaseAddr, dwModuleSize, pszSearchPath);
}

bool SymbolReader::LoadFile(CString FileName, CString FilePath, size_t dwBaseAddr, DWORD dwModuleSize, const TCHAR* pszSearchPath)
{
	m_strFileName = FileName;
	m_strFilePath = FilePath;

	m_dwModuleBase = dwBaseAddr;
	m_dwModuleSize = dwModuleSize;

	m_bInitialized = LoadSymbolData(pszSearchPath);

	return m_bInitialized;
}