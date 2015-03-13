#!/usr/bin/perl -w

use 5.010;
use strict;

for my $file (<*>) {
    if (-l $file) {
        my $target = readlink $file;
        print "$file -> $target\n";
    }
}
