use LWP::Simple;
use English;
use locale;

my $url = 'http://l2.overworld.su/index/statistics/fortress/1/';

my $content = get $url;
die "Couldn't get $url" unless defined $content;

my @lines = split /^/, $content;

($sl,$ml,$hl) = localtime();

for ($i=0;$i < scalar @lines;$i++)
{
    if ($lines[$i]=~/(\<td class=\"right\"\>\<strong\>)(.+)(\<\/strong\>\<br \/\>)/ )
    {
       my $name = $2;

       if ($lines[$i+4]=~/(.{19})(.+)/)
       {
          my $date =$2;
          $date =~ /(\d+)\:(\d+)\s/;
          my $h = $1-2+4+1;
          my $m = $2;
          
          my $tl = $hl*60+$ml;
          my $t  = $h*60+$m;
          if ($tl-$t<50 && $tl>$t)
          {
             print "Should be $name at $h:$m\n";
          }
          if ($tl<$t && $t-$tl<60)
          {
             print "Expected soon $name at $h:$m\n";
          }
       }
    }
}