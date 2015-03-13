#!/usr/bin/perl

while (<>) {
    if (/(.*wilma.*fred.*|.*fred.*wilma.*)/) {
        print $_;
    }
}

