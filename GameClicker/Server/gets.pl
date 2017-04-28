#!/usr/bin/perl

use CGI::Carp qw(fatalsToBrowser);
use CGI qw/:standard/;

use English;
$link = 'ss';
$val = param('prod') || 'Link';

open(IN,'<../updates.txt');
while(<IN>)
{
   $link='bb';
   if (s/$val\s*\=\s*//)
   {
      $link = $_;
      last;
   }
};

close(IN);

    print "Content-type: text/html\n\n";
    print '<html><head>';
    print "<Meta HTTP-EQUIV='Refresh' Content='1;Url=$link'>";
    print "</head><body><p><a href='$link'>Click here</a> if download did not start.</body></html>";
exit 0;
