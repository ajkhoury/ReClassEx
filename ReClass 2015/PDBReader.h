#pragma once

#include <dia2.h>
#include <DbgHelp.h>
#pragma comment(lib, "Dbghelp.lib")

// Basic types
const TCHAR* const rgBaseType[] =
{
	_T("<NoType>"),                         // btNoType = 0,
	_T("void"),                             // btVoid = 1,
	_T("char"),                             // btChar = 2,
	_T("wchar_t"),                          // btWChar = 3,
	_T("signed char"),
	_T("unsigned char"),
	_T("int"),                              // btInt = 6,
	_T("unsigned int"),                     // btUInt = 7,
	_T("float"),                            // btFloat = 8,
	_T("<BCD>"),                            // btBCD = 9,
	_T("bool"),                             // btBool = 10,
	_T("short"),
	_T("unsigned short"),
	_T("long"),                             // btLong = 13,
	_T("unsigned long"),                    // btULong = 14,
	_T("__int8"),
	_T("__int16"),
	_T("__int32"),
	_T("__int64"),
	_T("__int128"),
	_T("unsigned __int8"),
	_T("unsigned __int16"),
	_T("unsigned __int32"),
	_T("unsigned __int64"),
	_T("unsigned __int128"),
	_T("<currency>"),                       // btCurrency = 25,
	_T("<date>"),                           // btDate = 26,
	_T("VARIANT"),                          // btVariant = 27,
	_T("<complex>"),                        // btComplex = 28,
	_T("<bit>"),                            // btBit = 29,
	_T("BSTR"),                             // btBSTR = 30,
	_T("HRESULT")                           // btHresult = 31
};

// Tags returned by Dia
const TCHAR* const rgTags[] =
{
	_T("(SymTagNull)"),                     // SymTagNull
	_T("Executable (Global)"),              // SymTagExe
	_T("Compiland"),                        // SymTagCompiland
	_T("CompilandDetails"),                 // SymTagCompilandDetails
	_T("CompilandEnv"),                     // SymTagCompilandEnv
	_T("Function"),                         // SymTagFunction
	_T("Block"),                            // SymTagBlock
	_T("Data"),                             // SymTagData
	_T("Annotation"),                       // SymTagAnnotation
	_T("Label"),                            // SymTagLabel
	_T("PublicSymbol"),                     // SymTagPublicSymbol
	_T("UserDefinedType"),                  // SymTagUDT
	_T("Enum"),                             // SymTagEnum
	_T("FunctionType"),                     // SymTagFunctionType
	_T("PointerType"),                      // SymTagPointerType
	_T("ArrayType"),                        // SymTagArrayType
	_T("BaseType"),                         // SymTagBaseType
	_T("Typedef"),                          // SymTagTypedef
	_T("BaseClass"),                        // SymTagBaseClass
	_T("Friend"),                           // SymTagFriend
	_T("FunctionArgType"),                  // SymTagFunctionArgType
	_T("FuncDebugStart"),                   // SymTagFuncDebugStart
	_T("FuncDebugEnd"),                     // SymTagFuncDebugEnd
	_T("UsingNamespace"),                   // SymTagUsingNamespace
	_T("VTableShape"),                      // SymTagVTableShape
	_T("VTable"),                           // SymTagVTable
	_T("Custom"),                           // SymTagCustom
	_T("Thunk"),                            // SymTagThunk
	_T("CustomType"),                       // SymTagCustomType
	_T("ManagedType"),                      // SymTagManagedType
	_T("Dimension"),                        // SymTagDimension
	_T("CallSite"),                         // SymTagCallSite
	_T("InlineSite"),                       // SymTagInlineSite
	_T("BaseInterface"),                    // SymTagBaseInterface
	_T("VectorType"),                       // SymTagVectorType
	_T("MatrixType"),                       // SymTagMatrixType
	_T("HLSLType"),                         // SymTagHLSLType
	_T("Caller"),                           // SymTagCaller,
	_T("Callee"),                           // SymTagCallee,
	_T("Export"),                           // SymTagExport,
	_T("HeapAllocationSite"),               // SymTagHeapAllocationSite
	_T("CoffGroup")                         // SymTagCoffGroup
};

const TCHAR* const rgDataKind[] =
{
	_T("Unknown"),
	_T("Local"),
	_T("Static Local"),
	_T("Param"),
	_T("Object Ptr"),
	_T("File Static"),
	_T("Global"),
	_T("Member"),
	_T("Static Member"),
	_T("Constant")
};

const TCHAR* const rgLocationTypeString[] =
{
	_T("NULL"),
	_T("static"),
	_T("TLS"),
	_T("RegRel"),
	_T("ThisRel"),
	_T("Enregistered"),
	_T("BitField"),
	_T("Slot"),
	_T("IL Relative"),
	_T("In MetaData"),
	_T("Constant")
};

const TCHAR* const rgUdtKind[] =
{
	_T("struct"),
	_T("class"),
	_T("union"),
	_T("interface")
};

const TCHAR* const rgCallingConvention[] =
{
	_T("CV_CALL_NEAR_C      "),
	_T("CV_CALL_FAR_C       "),
	_T("CV_CALL_NEAR_PASCAL "),
	_T("CV_CALL_FAR_PASCAL  "),
	_T("CV_CALL_NEAR_FAST   "),
	_T("CV_CALL_FAR_FAST    "),
	_T("CV_CALL_SKIPPED     "),
	_T("CV_CALL_NEAR_STD    "),
	_T("CV_CALL_FAR_STD     "),
	_T("CV_CALL_NEAR_SYS    "),
	_T("CV_CALL_FAR_SYS     "),
	_T("CV_CALL_THISCALL    "),
	_T("CV_CALL_MIPSCALL    "),
	_T("CV_CALL_GENERIC     "),
	_T("CV_CALL_ALPHACALL   "),
	_T("CV_CALL_PPCCALL     "),
	_T("CV_CALL_SHCALL      "),
	_T("CV_CALL_ARMCALL     "),
	_T("CV_CALL_AM33CALL    "),
	_T("CV_CALL_TRICALL     "),
	_T("CV_CALL_SH5CALL     "),
	_T("CV_CALL_M32RCALL    "),
	_T("CV_ALWAYS_INLINED   "),
	_T("CV_CALL_NEAR_VECTOR "),
	_T("CV_CALL_RESERVED    ")
};

#define MAXELEMS(x)     (sizeof(x)/sizeof(x[0]))
#define SafeDRef(a, i)  ((i < MAXELEMS(a)) ? a[i] : _T("(none)"))

class PDBReader
{
public:
	PDBReader();

	bool IsInitialized() { return m_bInitialized; }

	bool LoadFile(CString FilePath);
	bool GetSymbolStringWithVA(size_t dwRVA, CString& outString);

private:
	void ReadSymTag(DWORD dwSymTag, CString& outString);

	void ReadName(IDiaSymbol *pSymbol, CString& outString);

	void ReadUndName(IDiaSymbol *pSymbol, CString& outString);

	void ReadBound(IDiaSymbol *pSymbol, CString& outString);

	void ReadData(IDiaSymbol *pSymbol, CString& outString);

	void ReadUDT(IDiaSymbol *pSymbol, CString& outString);

	void ReadUdtKind(IDiaSymbol *pSymbol, CString& outString);

	void ReadSymbolType(IDiaSymbol *pSymbol, CString& outString);

	void ReadType(IDiaSymbol *pSymbol, CString& outString);

	void ReadVariant(VARIANT var, CString& outString);

	void ReadLocation(IDiaSymbol *pSymbol, CString& outString);

	void ReadSymbol(IDiaSymbol *pSymbol, CString& outString);

	bool LoadDataFromPdb(const wchar_t* szFilename);

	IDiaDataSource* m_pSource;
	IDiaSession*	m_pSession;
	IDiaSymbol*		m_pGlobal;

	DWORD			m_dwMachineType;


	bool m_bInitialized;

	size_t m_ModuleBase;
	size_t m_ModuleSize;

	size_t m_ModuleBaseReturned;

};

extern PDBReader pdb;
