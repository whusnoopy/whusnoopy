#!/usr/bin/perl -w

use 5.010;
use strict;
use File::Basename;
use File::Spec;

my $softlink = 0;
if ($ARGV[0] eq "-s") {
    $softlink = 1;
    shift @ARGV;
}

my ($src, $tar) = @ARGV;
if (-d $tar) {
    $tar = File::Spec->catfile($tar, basename $src);
}

if ($softlink) {
    symlink $src, $tar or warn "Can not link $tar to $src: $!";
} else {
    link $src, $tar or warn "Can not link $tar to $src: $!";
}
say "link $tar to $src"
