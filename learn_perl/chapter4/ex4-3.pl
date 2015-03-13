#!/usr/bin/perl

sub average {
    $sum = 0;
    foreach (@_) {
        $sum += $_;
    }
    $num = @_;
    $average = $sum / $num;
}

sub above_average {
    my ($avg) = &average(@_);
    my ($cnt) = 0;
    foreach (@_) {
        if ($_ > $avg) {
            $cnt++;
        }
    }
    return $cnt;
}


my @fred = 1..1000;
my $num = &above_average(@fred);
print "The total num above the average of 1 to 1000 is $num.\n"

