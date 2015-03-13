#!/usr/bin/perl

print "Please input a string and a number how many times you want to repeat the string:\n";

chomp ($a = <STDIN>);
chomp ($b = <STDIN>);

print "$a\n" x $b;

