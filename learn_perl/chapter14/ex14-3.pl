#!/usr/bin/perl -w

use 5.010;
use strict;

print "Input a string:\n";
chomp (my $line = <>);

while (1) {
    print "input which sub string you want to search (or nothing to exit): ";
    chomp (my $substr = <>);
    last if !$substr or $substr =~ /^\s+$/;
    my $start = 0;
    while (index($line, $substr, $start) >= 0) {
        $start = index($line, $substr, $start);
        print $start, " ";
        $start++;
    }
    print "\n";
}

