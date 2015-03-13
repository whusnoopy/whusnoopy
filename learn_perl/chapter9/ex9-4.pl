#!/usr/bin/perl -w

use strict;

chomp(my $year = `date +%Y`);
$^I = ".bak";

while (<>) {
    if (/^#!/) {
        $_ .= "## Copyright (c) $year by whusnoopy\n";
    }
    print;
}
