#!/usr/bin/perl -w

use strict;

# This next line of code is used when you get to Chapter 9.
my $what = 'fred|barney';

while (<>) {
    chomp;
    # If you want to try matching strings which may contain
    # newlines, here's the trick to use: Uncomment this next
    # line, then use a pound sign ("#") wherever you mean to
    # have a newline within your data string.
    # s/#/\n/g;

    if (/\s+$/) {
        print "$_\$\n"
    } else {
        print "No match.\n";
    }

}
