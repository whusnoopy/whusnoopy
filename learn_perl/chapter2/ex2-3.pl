#!/usr/bin/perl

$pi = 3.1415926;
chomp ($r = <STDIN>);
if ($r < 0 ) {
	print "0\n";
} else {
	$c = $r * 2 * $pi;
	print $c."\n";
}
