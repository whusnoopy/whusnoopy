#!/usr/bin/perl -w

use 5.010;
use strict;

my %last_name = qw{ 
	fred flintstone Wilma Flintstone Barney Rubble
	betty rubble Bamm-Bamm Rubble PEBBLES FLINTSTONE
};

sub by_name {
    "\L$last_name{$a}" cmp "\L$last_name{$b}"
        or
    "\L$a" cmp "\L$b"
}

foreach my $name (sort by_name keys %last_name) {
    printf "%10s %10s\n", $name, $last_name{$name};
}

