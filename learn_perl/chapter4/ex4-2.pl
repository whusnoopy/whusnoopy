#!/usr/bin/perl

sub total {
    $count = 0;
    foreach (@_) {
        $count += $_;
    }
    $count;
}

my @fred = 1..1000;
my $fred_total = &total(@fred);
print "The total of 1 to 1000 is $fred_total.\n";

