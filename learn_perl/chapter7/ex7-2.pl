#!/usr/bin/perl

while (<>) {
    if (/[fF]red/) {
        print $_;
    }
}
