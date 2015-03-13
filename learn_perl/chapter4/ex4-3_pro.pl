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
    my (@res) = ();
    foreach (@_) {
        if ($_ > $avg) {
            $cnt++;
            push @res, $_;
        }
    }
    unshift @res, $cnt;
    return @res;
}


my @fred = 1..100;
my @res = &above_average(@fred);
my $num = shift(@res);

print "The total num above the average of 1 to 100 is $num.\n";
print "and these numbers are @res.\n";

