#!/usr/bin/perl -w

use strict;

my $sec_num = int(1 + rand 100);
my $round = 0;

while (1) {
    $round++;
    print "Input the number, or 'quit'/'exit' to end this game: ";
    chomp (my $guess = <>);
    if ($guess =~ /quit|exit|\D|^\s*$/i) {
        print "Invalid input or you select quit, end this game, and the number is $sec_num\n";
        last;
    } elsif ($guess > $sec_num) {
        print "The number you guess is large than the secret number, try again :)\n";
    } elsif ($guess < $sec_num) {
        print "The number you guess is less than the secret number, try again :)\n";
    } elsif ($guess == $sec_num) {
        print "You got the right number, you guessed $round times.\n";
        last;
    }
}

