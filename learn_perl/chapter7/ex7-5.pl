#!/usr/bin/perl

use 5.010;

while (<>) {
    if (/([\S])\g{1}/) {
        print $_;
    }
}
