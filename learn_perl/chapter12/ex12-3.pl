#!/usr/bin/perl -w

use strict;
use 5.010;

die "give me some files during arguments" unless @ARGV;

for  (@ARGV) {
    if ( -o -r -e ) {
        say "$_ is owned by you and is readable and writeable";
    }
}

