#!/usr/bin/perl

use 5.010;

say "input some words in seperate lines:";
chomp (my @words = <STDIN>);

foreach $word (@words) {
    $word_count{$word}++;
}

foreach $word (sort keys %word_count) {
    say "$word appears $word_count{$word} times";
}

