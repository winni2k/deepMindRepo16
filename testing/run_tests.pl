#!/usr/bin/perl
# run_tests.pl     wkretzsch@gmail.com     2015/06/27 09:25:02

use warnings;
use strict;

for my $test (@ARGV) {
    $test = "./$test";
    system($test) == 0
      or die "Test $test failed: $?";
}
