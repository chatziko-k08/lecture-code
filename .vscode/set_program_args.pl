#!/usr/bin/perl
use strict;

# Auxiliary program for changing settings.json arguments via a VS Code task

# we get the args from vscode via an env variable, to avoid escaping issues
my @args = split / +/, $ENV{ARGS};

s/"/\\"/g	for @args;	# escape double quotes in json
push @args, ('#')x10;	# fill with '#' at the end

# read settings.json
$/ = undef;		# read the whole file at once
open my $fh, '<', 'settings.json';
my $settings = <$fh>;
close $fh;

# replace "argN": "..." in JSON with the new value
$settings =~ s/"arg(\d)"(: \s*)".*"
	          /"arg$1"$2"$args[$1-1]"/xg;

# save settings.json
open $fh, '> settings.json';
print $fh $settings;
close $fh;

print "Args changed to: $ENV{ARGS}\n";
