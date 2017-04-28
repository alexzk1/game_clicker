use English;
use File::Copy;
use Shell qw(getver);

$pt_dll = 'D:\UTILS\Qt\SDK1.1\Desktop\Qt\4.7.4\mingw\bin';
$dst = ".\\win_release";


copy("$pt_dll\\QtCore4.dll","$dst\\QtCore4.dll");
#copy("$pt_dll\\QtScript4.dll","$dst\\QtScript4.dll");
copy("$pt_dll\\QtGui4.dll","$dst\\QtGui4.dll");
copy("$pt_dll\\QtNetwork4.dll","$dst\\QtNetwork4.dll");
#copy("$pt_dll\\QtWebKit4.dll","$dst\\QtWebKit4.dll");
copy("$pt_dll\\QtXml4.dll","$dst\\QtXml4.dll");
#copy("$pt_dll\\QtXmlPatterns4.dll","$dst\\QtXmlPatterns4.dll");
#copy("$pt_dll\\QtDeclarative4.dll","$dst\\QtDeclarative4.dll");
#copy("$pt_dll\\QtSql4.dll","$dst\\QtSql4.dll");

copy("$pt_dll\\phonon4.dll","$dst\\phonon4.dll");
copy("$pt_dll\\libgcc_s_dw2-1.dll","$dst\\libgcc_s_dw2-1.dll");
copy("$pt_dll\\mingwm10.dll","$dst\\mingwm10.dll");

open(OUT, ">Ver.nsi");
print OUT  '!define PRODUCT_VERSION "'.getver('.\GameClicker.exe')."\"\n";
close(OUT);