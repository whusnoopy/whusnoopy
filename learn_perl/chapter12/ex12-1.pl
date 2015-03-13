#!/usr/bin/perl -w

use strict;
use 5.010;

sub checkFile {
    my $filename = shift @_;
    say "now check $filename";
    unless ( -e $filename ) {
        say "  $filename doesn't exists";
        return;
    }
    (-r $filename) ? say "  readable" : say "  UNreadable";
    (-w $filename) ? say "  writeable" : say "  UNwriteable";
    (-x $filename) ? say "  executableable" : say "  UNexecutableable";
}

for my $filename (@ARGV) {
    &checkFile($filename);
}

