#!/usr/bin/perl

while (<>) {
    if (/[A-Z][a-z]+/) {
        print $_;
    }
}
