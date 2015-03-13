#!/usr/bin/perl

chomp (@lines = <STDIN>);
@lines = sort @lines;
print "@lines\n";

foreach $line (@lines) {
	print "$line\n";
}
