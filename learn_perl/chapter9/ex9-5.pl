#!/usr/bin/perl -w

use strict;

my %filelist;
foreach (@ARGV) {
    $filelist{$_} = 1;
}

while (<>) {
    if (/## Copyright/) {
        delete $filelist{$ARGV};
    }
}

@ARGV = sort keys %filelist;
if (@ARGV == 0) {
    exit;
}

chomp(my $year = `date +%Y`);
$^I = ".bak";

while (<>) {
    if (/^#!/) {
        $_ .= "## Copyright (c) $year by whusnoopy\n";
    }
    print;
}
