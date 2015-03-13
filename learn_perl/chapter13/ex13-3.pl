#!/usr/bin/perl -w

use 5.010;
use strict;

print "Input a dir path where you want to go: ";
chomp (my $target_dir = <>);

if ($target_dir =~ /^\s*$/) {
    chdir or die "Can not move to your home dir: $!";
} else {
    chdir $target_dir or die "Can not move to $target_dir: $!";
}

opendir DH, "." or die "Can not open current dir: $!";

my @all_files = sort (readdir DH);
closedir DH;

print "In dir $target_dir, we have these files:\n";
print "@all_files\n";

