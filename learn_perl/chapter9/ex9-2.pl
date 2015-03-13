#!/usr/bin/perl -w
## Copyright (c) 2011 by whusnoopy

use strict;

if (@ARGV < 1) {
    die "Not enough arguments\n";
}

my $input_filename = $ARGV[0];

if (! open IN, $input_filename) {
    die "Can not open input file [$input_filename]\n";
}

my $outfile = $input_filename;
$outfile =~ s/$/.out/;

if (! open OUT, "> $outfile") {
    die "Can not open output file [$outfile]\n";
}

while (<IN>) {
    s/fred/Larry/gi;
    print OUT $_;
}
