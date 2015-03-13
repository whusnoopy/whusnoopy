#!/usr/bin/perl

use 5.010;

say "Input lines separately, and use Ctrl-D to finish:";
chomp (@lines = <STDIN>);

say "123456789012345678901234567890123456789012345678901234567890";
foreach $line (@lines) {
    printf "%20s\n", $line;
}

