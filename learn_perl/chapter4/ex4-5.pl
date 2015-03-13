#!/usr/bin/perl

sub greet {
    if (defined(@already_here)) {
        my $this_one = shift @_;
        print "Hi $this_one! I've seen: @already_here\n";
        push @already_here, $this_one;
    } else {
        my $this_one = shift @_;
        print "Hi $this_one! You are the first one here!\n";
        push @already_here, $this_one;

    }
}

greet ( "Fred" );
greet ( "Barney" );
&greet ( "Wilma" );
&greet ( "Betty" );

