#!/usr/bin/perl -w

use strict;
use 5.010;

my $oldest_file = "";
my $days = 0;

for my $filename (@ARGV) {
    if ( -e $filename ) {
        my $exists_time = (-M $filename);
        if ( $exists_time > $days ) {
            $oldest_file = $filename;
            $days = $exists_time;
        }
    }
}

if ( $oldest_file ) {
    say "the oldest file is $oldest_file, and it created on $days days ago";
} else {
    say "no such files";
}

