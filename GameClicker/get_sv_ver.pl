open(V, "<../Versions.h") or die "Can't open header with version\n";
while(<V>)
{
       if (s/\s*\#define CLICKER_VER\s+\"//)
       {
          s/[\"\n\r]//g;
          $v=$_;
          last;
       }
}
close(V);
print "$v";
