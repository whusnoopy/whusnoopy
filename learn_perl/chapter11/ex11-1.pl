#!/usr/bin/perl -w

use strict;
use Module::CoreList;

my %modules = %{ $Module::CoreList::version{5.008} };

foreach my $key (sort keys %modules) {
    printf "%20s : %s\n", $key, $modules{$key};
}

