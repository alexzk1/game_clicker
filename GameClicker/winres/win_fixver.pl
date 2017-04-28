use English;

$n = $ARGV[1] || die("Exe name not set\n");
$ico = $ARGV[2] || die("Icon not set\n");
$cmd = "$ARGV[3] |";
open(fh, $cmd) || die("Can't open version getter $cmd\n");
while (<fh>) {
 $v = $_;
 last;
}
close fh;
$v =  $v || die("Version not set\n");


$src = $ARGV[4] || die "Source exe is not set\n";

if (open (IN,"<$ARGV[0]"))
{
  
  print "Reading: $ARGV[0]\n";
  @f = <IN>;
  close(IN);  
  $a = $v;
  $a=~s/\./\,/g;
  print "$a\n";
  print "Writting: $ARGV[0]\n";
  if (open(OUT, ">$ARGV[0]"))
  {

    foreach (@f)
    {
      if (m/\.manifest/)
      {
         #$_ = "1 24 $n.manifest\n"; 
      }
      $_ = "FILEVERSION $a\n" if (m/^FILEVERSION/);
      $_ = "PRODUCTVERSION $a\n" if (m/^PRODUCTVERSION/);
      $_ = "\t\tVALUE \"FileVersion\", \"$v\"\n" if (m/VALUE \"FileVersion\"/);
      $_ = "\t\tVALUE \"ProductVersion\", \"$v\"\n" if (m/VALUE \"ProductVersion\"/);

      $_ = "\t\tVALUE \"FileDescription\", \"$n\"\n" if (m/VALUE \"FileDescription\"/);
      $_ = "\t\tVALUE \"InternalName\", \"$n\"\n" if (m/VALUE \"InternalName\"/);
      $_ = "\t\tVALUE \"OriginalFilename\", \"$n\.exe\"\n" if (m/VALUE \"OriginalFilename\"/);
      $_ = "\t\tVALUE \"ProductName\", \"$n\"\n" if (m/VALUE \"ProductName\"/);

      print OUT $_;
    };
    close (OUT);
  }
}

open (OUT,">win_res_icon.ini");

print OUT<<ENDL;

[FILENAMES]
Exe=      $src
SaveAs=   $n.exe
Log = win_res.log

[COMMANDS]
-addoverwrite "$ico", ICONGROUP,MAINICON,0
-addoverwrite ".\\vers.res", VERSIONINFO,1,
-addoverwrite gen.manifest,24,1,1033

ENDL

close (OUT);

open (OUT,'>gen.manifest');

print OUT<<ENDL;
<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<assembly xmlns="urn:schemas-microsoft-com:asm.v1" manifestVersion="1.0">
  <assemblyIdentity version="$v"
   processorArchitecture="X86"
   name="SmartSniffer.$n"
   type="win32"/>
  <description>SmartSniffer Project's Part</description>
  <trustInfo xmlns="urn:schemas-microsoft-com:asm.v2">
  <security>
   <requestedPrivileges>
    <requestedExecutionLevel level="requireAdministrator" uiAccess="false"/>
   </requestedPrivileges>
  </security>
  </trustInfo>
</assembly>
ENDL
close (OUT);