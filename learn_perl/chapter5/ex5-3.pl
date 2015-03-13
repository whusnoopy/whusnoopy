#!/usr/bin/perl

use 5.010;

say "Input width you want to use:";
chomp ($width = <STDIN>);

say "Input lines separately, and use Ctrl-D to finish:";
chomp (@lines = <STDIN>);

say "1234567890" x (($width + 9)/10); 
foreach $line (@lines) {
    printf "%${width}s\n", $line;
}

