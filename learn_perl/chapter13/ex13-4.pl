#!/usr/bin/perl -w

use 5.010;
use strict;

foreach my $file (@ARGV) {
    unlink $file;
    say "removed $file";
}
