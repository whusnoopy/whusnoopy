#!/usr/bin/perl

@names = qw/ fred betty barney dino wilma pebbles bamm-bamm /;

@idx = <STDIN>;
foreach $i (@idx) {
	print @names[$i-1]."\n";
}
