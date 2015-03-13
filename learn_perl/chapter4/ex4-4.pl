#!/usr/bin/perl

sub greet {
    if (defined($last_one)) {
        my $this_one = shift @_;
        print "Hi $this_one! $last_one is also here!\n";
        $last_one = $this_one;
    } else {
        $last_one = shift @_;
        print "Hi $last_one! You are the first one here!\n";
    }
}

greet ( "Fred" );
greet ( "Barney" );

