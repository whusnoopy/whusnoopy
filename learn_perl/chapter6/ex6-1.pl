#!/usr/bin/perl

use 5.010;

my %firstname = (
    "fred" => "flintstone",
    "barney" => "fubble",
    "wilma" => "flintstone",
);

say "whose firstname you want to known?";
while (chomp($name = <STDIN>)) {
    if (! exists $firstname{$name}) {
        say "sorry, I think I never knew someone called $name";
    } else {
        say "${name}'s firstname is $firstname{$name}";
    }
    say "do you want to know some other's firstname? input his name or Ctrl-D to end this program";
}

