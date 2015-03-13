#!/usr/bin/perl

use 5.010;

$ENV{ZERO} = 0;
$ENV{EMPTY} = "";
$ENV{UNDEFINED} = undef;

my $max_length = 0;
foreach $key (keys %ENV) {
    $this_length = length ($key);
    if ($this_length > $max_length) {
        $max_length = $this_length;
    }
}

foreach $key (sort keys %ENV) {
    printf "%-${max_length}s %s\n", $key, $ENV{$key} // "undefined";
}

