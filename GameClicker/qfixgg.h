#ifndef QFIXGG_H
#define QFIXGG_H

#include <QString>
#include <list>
#include <vector>
#include <memory>
#include <QtCore>
#include "fake_types.h"




/*
 * GameGuard may change DLLs in space of LA2, so that must be restored, so can be used
 */
class QFixGG
{
private:
    typedef HMODULE (__stdcall *typeLL)(const char*);
    HMODULE hDll;
    QString FullPath;
    volatile std::shared_ptr<uint8_t> Stub;
    DWORD OrigFunc;
    long SameAt;

    QString FFunc;
    QString FLibrary;
    bool ready;
protected:
    void          init(QString Func, QString Library)  __attribute__((optimize("-O0")));
    void          clean();
    typedef BOOL (WINAPI *LPFN_ISWOW64PROCESS) (HANDLE, PBOOL);
    LPFN_ISWOW64PROCESS fnIsWow64Process;
    BOOL IsWow64();

public:
    QFixGG(QString Func, QString Library);
    QFixGG();
    QFixGG(const QFixGG& C);
    QFixGG operator = (const QFixGG& C);

    ~QFixGG();

    unsigned long call(int count,...)const  __attribute__((optimize("-O0")));
    unsigned long callfromlua(lua_State* L)const  __attribute__((optimize("-O0")));

    bool          isready() const;
};


typedef unsigned short ushort;
typedef unsigned long  ulong;
typedef unsigned char  uchar;

#ifdef Q_OS_WIN32
struct PEHeader
{
 ushort Machine;
 ushort NumberOfSections;
 ulong  TDStamp;
 ulong  pSymbols;
 ulong  NumberOfSymbols;
 ushort SizeOfOptionalHeader;
 ushort Characteristics;
};

struct PEOptHeader
{
 ushort Magic;
 uchar  MJLVer;
 uchar  MNLVer;
 ulong  SizeOfCode;
 ulong  SizeOfInitializedData;
 ulong  SizeOfUninitializedData;
 ulong  AddressOfEntryPoint;
 ulong  BaseOfCode;
 ulong  BaseOfData; //NOT FOR PE32+!!!

 //NT header part
 ulong  ImageBase;
 ulong  SectionAlignment;
 ulong  FileAlignment;
 ushort MajorOperatingSystemVersion;
 ushort MinorOperatingSystemVersion;
 ushort MajorImageVersion;
 ushort MinorImageVersion;
 ushort MajorSubsystemVersion;
 ushort MinorSubsystemVersion;
 ulong  Reserved;
 ulong  SizeOfImage;
 ulong  SizeOfHeaders;
 ulong  CheckSum;
 ushort Subsystem;
 ushort DLLCharacteristics;
 ulong  SizeOfStackReserve;
 ulong  SizeOfStackCommit;
 ulong  SizeOfHeapReserve;
 ulong  SizeOfHeapCommit;
 ulong  LoaderFlags;//obsolete
 ulong  NumberOfRvaAndSizes;
};

struct TSectionHeader
{
 uchar  Name[8];
 ulong  VirtualSize;
 ulong  VirtualAddress;
 ulong  SizeOfRawData;   //size of section on DISK - into file
 ulong  PointerToRawData;
 ulong  PointerToRelocations;
 ulong  PointerToLinenumbers;
 ushort NumberOfRelocations;
 ushort NumberOfLinenumbers;
 ulong  Characteristics;
};


typedef std::vector<IMAGE_DATA_DIRECTORY> TDirs;
typedef std::vector<TSectionHeader> THeaders;

class PESplit //constructs header info from got data
{
private:
 ulong HoleSizeOfHeader; //size of HOLE headers...so offset of sections?
 uchar *DosStub;
 ulong  DosSize;
public:
 //header(s) of PE32
 PEHeader    Header;
 PEOptHeader OptHeader;
 TDirs       Dirs;
 THeaders    SectionHeaders; //sections into PE....
 PESplit(const char* FileName);
 virtual ~PESplit();

 long Rva2Off(ulong Rva); //-1 if not exists in file in section!
 void Split(void *Data,ulong Size);
 void Load(const char* FileName);
};
#ifndef RALIGN
#define RALIGN(dwToAlign, dwAlignOn) ( (!dwToAlign)?dwAlignOn:((dwToAlign % dwAlignOn == 0) ? dwToAlign : dwToAlign - (dwToAlign % dwAlignOn) + dwAlignOn))
 // VirtualSize = RALIGN(SizeOfRawData, SectionAlignment);
#endif
#endif

#endif // QFIXGG_H
