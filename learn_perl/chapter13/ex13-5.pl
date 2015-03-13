#!/usr/bin/perl -w

use 5.010;
use strict;
use File::Basename;
use File::Spec;

my ($src, $tar) = @ARGV;
if (-d $tar) {
    $tar = File::Spec->catfile($tar, basename $src);
}

rename $src, $tar;
say "move $src to $tar"
