#!/usr/bin/perl -w

use 5.010;
use strict;

my @numbers;
push @numbers, split while <>;
print @numbers;

my $max_len = 0;
foreach my $num (@numbers) {
    $max_len = length("".$num) if length("".$num) > $max_len;
}

@numbers = sort {$a <=> $b} @numbers;
foreach my $num (@numbers) {
    printf "%${max_len}g\n", $num;
}
