#!/usr/bin/perl

print "Please input two numbers in two lines:\n";
chomp ($a = <STDIN>);
chomp ($b = <STDIN>);

$c = $a * $b;
print "$a by $b is $c\n";
