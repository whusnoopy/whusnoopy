#!/usr/bin/perl -w

use 5.010;
use strict;

print "Input a dir path where you want to go: ";
chomp (my $target_dir = <>);

if ( $target_dir ) {
    chdir $target_dir or die "Can not move to $target_dir";
} else {
    $target_dir = "~/";
    chdir or die "Can not move to ~/";
}

my @all_files = glob ".* *";
print "In dir $target_dir, we have these files:\n";
print "@all_files\n";

