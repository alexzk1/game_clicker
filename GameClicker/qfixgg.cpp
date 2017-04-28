#include "qfixgg.h"
#include <QDebug>
#include <QFile>
#include <algorithm>
#include <QtWidgets/QMessageBox>
#include <QLibrary>
#pragma GCC push_options
#pragma GCC optimize("O0")

#ifdef Q_OS_WIN32
#include "detours.h"
extern "C" PVOID DetourCopyInstruction(PVOID pDst,
                                       PVOID *ppDstPool,
                                       PVOID pSrc,
                                       PVOID *ppTarget,
                                       LONG *plExtra);


#endif

QFixGG::QFixGG(QString Func, QString Library)
{
    hDll = nullptr;
    Stub = nullptr;
    ready= false;
    init(Func,Library);
}

QFixGG::QFixGG(const QFixGG& C)
{
    FLibrary = FFunc = "";
    hDll = nullptr;
    Stub = nullptr;
    ready= false;
    init(C.FFunc,C.FLibrary);
}

QFixGG::QFixGG()
{
    FLibrary = FFunc = "";
    hDll = nullptr;
    Stub = nullptr;
    ready= false;
}

bool QFixGG::isready() const
{
    return ready;
}

BOOL QFixGG::IsWow64()
{
    BOOL bIsWow64 = FALSE;

    //IsWow64Process is not available on all supported versions of Windows.
    //Use GetModuleHandle to get a handle to the DLL that contains the function
    //and GetProcAddress to get a pointer to the function if available.

    fnIsWow64Process = (LPFN_ISWOW64PROCESS) GetProcAddress(
                           GetModuleHandle(TEXT("kernel32")),"IsWow64Process");

    if(NULL != fnIsWow64Process)
    {
        if (!fnIsWow64Process(GetCurrentProcess(),&bIsWow64))
        {
            //handle error
        }
    }
    return bIsWow64;
}


void QFixGG::init(QString Func, QString Library)
{
    FFunc = Func;
    FLibrary = Library;
    ready = false;
    if (Func == "" || Library=="")
        return;
    if (!hDll)
    {
        //wchar_t arr[2048];
        //memset(arr,0,2048*2);
        //Library.toWCharArray(arr);
        try{
            hDll = LoadLibraryA(Library.toLocal8Bit().data());
        }
        catch(...)
        {
            QMessageBox::critical(NULL,"Game Clicker","Exception on load "+Library);
        }
    }

    if (!Stub)
    {
        Stub.reset(new unsigned char[100], [](auto* p){if (p) delete []p;});
    }

    DWORD oldp;
    if (!VirtualProtect((void*)Stub,100,PAGE_EXECUTE_READWRITE,&oldp))
    {
        qDebug()<<"Failed to protect: "<<GetLastError();
    }

    if (hDll)
    {
        wchar_t fn[2048];
        GetModuleFileName(hDll,fn,2048);
        FullPath = QString().fromWCharArray(fn);
        //qDebug()<<FullPath;


        PESplit split(FullPath.toLocal8Bit().data());
        OrigFunc=0;
        ulong rva = (ulong)((quint32)(OrigFunc=(DWORD)GetProcAddress(hDll,Func.toAscii().data())) - (quint32)hDll);
        if (!OrigFunc)
        {
            QString err = QString("Err getting func %1 with code %2").arg(Func).arg(GetLastError());
            QMessageBox::critical(NULL,"Game Clicker",err);
            return;
        }
        long foffset = split.Rva2Off(rva);
        //qDebug()<<rva<<foffset<<OrigFunc;
        QFile fp(FullPath);
        fp.open(QIODevice::ReadOnly);
        fp.seek(foffset);
        unsigned char tmp[60];
        fp.read((char*)tmp,sizeof(tmp));
        fp.close();
        SameAt = 0;

        if (!IsWow64())
            for (int i=0;i<5;i++) //upto 5 instructions (not bytes)
            {
                DWORD target = 0;LONG extra = 0;
                DWORD next;
                try{
                    next = (DWORD)DetourCopyInstruction(NULL,NULL,SameAt+tmp,(PVOID *)&target,&extra);
                }catch(...)
                {
                    qDebug()<<"Detour exception\n";
                    SameAt = 0;
                    break;
                }

                if (DETOUR_INSTRUCTION_TARGET_NONE!=(PVOID)target)
                {
                    //qDebug()<<Func<<"Jump detected, to copy:"<<SameAt<<"bytes, instructions:"<<i;
                    break;
                }
                DWORD size = next - (SameAt+(DWORD)tmp);
                if (SameAt+size > sizeof(tmp))
                {
                    //qDebug()<<"Size limit";
                    break;
                }
                SameAt += size;
            }

        //всегда 5 байт с диска тащим
        //SameAt = 5;//а не пашет! там разные длины, таки придется дизасмить
        if (SameAt)
            memcpy((void*)Stub,tmp,SameAt);
        //if (Stub[0]==0xE9 || Stub[0]==0xEB || Stub[0]==0xFF || Stub[0]==0xEA)
        // SameAt = 0; //it has relative jump as 1st instruction, better not touch it

        Stub[SameAt] = 0xEA;
        *(DWORD*)(Stub+SameAt+1) = OrigFunc+SameAt;
        asm volatile(
                    ".intel_syntax noprefix;"
                    "mov [%0],CS;"
                    ".att_syntax\n"
                    :
                    :"r"(Stub+SameAt+5)
                    );
        ready = true;
    }
    else
        QMessageBox::critical(NULL,"Game Clicker","Failed to load "+Library);
}

void QFixGG::clean()
{
    Stub.reset();
    if (hDll && hDll!=INVALID_HANDLE_VALUE)
        FreeLibrary(hDll);
    hDll=NULL;
    FLibrary = FFunc = "";
    ready = false;
}

QFixGG::~QFixGG()
{
    clean();
}

QFixGG QFixGG::operator = (const QFixGG& C)
{
    clean();
    init(C.FFunc,C.FLibrary);
    return *this;
}

unsigned long QFixGG::callfromlua(lua_State* L)const
{
    if (!Stub || !isready())
        return -1;
    unsigned long result;
    int count = lua_gettop(L)-2;
    if (count)
    {
        quint32 *arr = new quint32[count];
        for (int i = 0;i<count;i++)
        {
            if (lua_isnumber(L,i+3))
            {
                arr[i] = lua_tointeger(L,i+3);
            }
            else
                if(lua_isstring(L,i+3))
                {
                    arr[i] = (quint32)lua_tostring(L,i+3);
                }
                else
                    if (lua_isboolean(L,i+3))
                    {
                        arr[i] = (quint32)lua_toboolean(L,i+3);
                    }
                    else
                    {
                        lua_pushstring(L,"Wrong type of parameter in DLL call");
                        lua_error(L);
                        return -1;
                    }
        }
        //__stdcall means right to left pushing, so we must push last element 1st
        asm volatile(
                    ".intel_syntax noprefix;"
                    "loop:;"
                    "mov %%eax, [%%edx+%%ecx*4-4];"
                    "push %%eax;"
                    "sub %%ecx,1;"
                    "jnz loop;"
                    "call %%ebx;"
                    ".att_syntax\n"
                    :"=a" (result)
                    :"c" (count), "d" (arr),"b"(Stub)
                    );
        delete []arr;
    }
    else
    {
        asm volatile(
                    ".intel_syntax noprefix;\n"
                    "call %%ebx;\n"
                    ".att_syntax\n"
                    :"=a" (result)
                    :"b"(Stub)
                    );
    }
    return result;
}

unsigned long QFixGG::call(int count,...)const
{
    if (!Stub || !isready())
        return -1;
    unsigned long result;
    if (count)
    {
        quint32 *arr = new quint32[count];
        va_list vl;
        va_start(vl,count);
        for (int i=0;i<count;i++)
        {
            arr[i]=va_arg(vl,quint32);
            //qDebug()<<arr[i];
        }
        va_end(vl);
        //volatile char p = *Stub; //otherwise release build crushes

        //__stdcall means right to left pushing, so we must push last element 1st
        asm volatile(
                    ".intel_syntax noprefix;"
                    "1:;"
                    "mov %%eax, [%%edx+%%ecx*4-4];"
                    "push %%eax;"
                    "sub %%ecx,1;"
                    "jnz 1b;"
                    "call %%ebx;"
                    ".att_syntax\n"
                    :"=a" (result)
                    :"c" (count), "d" (arr),"b"(Stub)
                    );
        delete []arr;
    }
    else
    {
        asm volatile(
                    ".intel_syntax noprefix;"
                    "call %%ebx;"
                    ".att_syntax\n"
                    :"=a" (result)
                    :"b"(Stub)
                    );
    }
    return result;
}

#ifdef Q_OS_WIN32
PESplit::PESplit(const char* FileName)
{
    DosStub=NULL;
    Load(FileName);
}


PESplit::~PESplit()
{
    if (DosStub) delete [] DosStub;
}

void PESplit::Load(const char* FileName)
{

    HANDLE fp=CreateFileA(FileName,GENERIC_READ,FILE_SHARE_WRITE|FILE_SHARE_READ,NULL,
                          OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
    DWORD k,r;
    uchar *buf=new uchar[r=GetFileSize(fp,NULL)];
               ReadFile(fp,buf,r,&k,NULL);
               CloseHandle(fp);
               Split(buf,k);
               delete []buf;
}

               long PESplit::Rva2Off(ulong rva)
    {
        for (ulong i=0;i<SectionHeaders.size();i++)
        {
            TSectionHeader hdr=SectionHeaders[i];
            if (rva>=hdr.VirtualAddress && rva<=hdr.VirtualAddress+hdr.VirtualSize)
            {
                rva-=hdr.VirtualAddress;
                if (rva>hdr.SizeOfRawData) return -1;
                return hdr.PointerToRawData+rva;
            }
        }
        return -1;
    }

    void PESplit::Split(void *Data,ulong Size)
    {
    memset(&Header,0,sizeof(Header));
    memset(&OptHeader,0,sizeof(OptHeader));
    Dirs.clear();SectionHeaders.clear();

    HoleSizeOfHeader=0;
    char *data=(char*)Data;
    //getting file offset
    ulong off=*((DWORD*)(data+0x3C));
    DosSize=off+4;
    DosStub=new uchar[DosSize];
    memcpy(DosStub,data,DosSize);

    if (off>Size || off+sizeof(Header)>Size) return;
    const char sign[]={'P','E',0,0};
    if (memcmp(data+off,sign,4)) return;
    memcpy(&Header,data+off+4,sizeof(Header));

    ulong optoff=off+4+sizeof(Header);
    if (optoff+sizeof(OptHeader)>Size) return;
    memcpy(&OptHeader,data+optoff,sizeof(OptHeader));

    if (OptHeader.Magic!=0x10b) return;

    ulong rva=optoff+sizeof(OptHeader);
    IMAGE_DATA_DIRECTORY ids;

    for (ulong i=0;i<OptHeader.NumberOfRvaAndSizes;i++)
    {
        ulong of=rva+sizeof(IMAGE_DATA_DIRECTORY)*i;
        if (of>Size) return;
        memcpy(&ids,data+of,sizeof(ids));
        Dirs.push_back(ids);
    }

    ulong sectableoff=rva+sizeof(IMAGE_DATA_DIRECTORY)*OptHeader.NumberOfRvaAndSizes;
    //таблица секций теперя....
    TSectionHeader shdr;

    for (ushort i=0;i<Header.NumberOfSections;i++)
    {
        ulong of=sectableoff+sizeof(TSectionHeader)*i;
        if (of>Size) return;
        memcpy(&shdr,data+of,sizeof(shdr));
        if (!shdr.VirtualSize) //fix virtual size if zero (Watcom compiler)
            shdr.VirtualSize=RALIGN(shdr.SizeOfRawData,OptHeader.SectionAlignment);
        SectionHeaders.push_back(shdr);
    }

    HoleSizeOfHeader=sectableoff+Header.NumberOfSections*sizeof(TSectionHeader);
}
#endif

#pragma GCC pop_options
