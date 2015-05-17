#!/usr/bin/env perl

#  Copyright (C) 2014 Julien Rabinow <jnr305@nyu.edu>
#
#  This file is part of Lab2-Scheduler.
#
#  Lab2-Scheduler is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation, either version 3 of the License, or
#  (at your option) any later version.
#
#  Lab2-Scheduler is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with Lab2-Scheduler. If not, see <http://www.gnu.org/licenses/>.

use warnings;
use strict;
use Getopt::Long;

# generate lookup table for object
sub LT_gen {
	my $file_handle = shift;
	my %object = @_;

	print $file_handle,
			"struct ", $object{"CLASS"}, "_LT lt = {\n",
			"\tNULL,\n",
			"\tfalse,\n";
	print $file_handle, "\t&$_->{NAME},\n" foreach(@{$object{"PROTOTYPES"}});
	print $file_handle, "};\n\n";
}

# generate builder struct for object
sub Builder_gen {
	my $file_handle = shift;
	my %object = @_;

	print $file_handle,
			"const struct Builder __", $object{"CLASS"}, "__ = {\n",
			"\t&", $object{"SUPER"}, ",\n",
			"\t(Lookup_Table) &lt,\n",
			"\tsizeof(struct ", $object{"CLASS"}, "),\n",
			"\t\"", $object{"CLASS"}, "\"\n",
			"};\n\n";
}

# print object method prototypes
sub prototype_gen {
	my $file_handle = shift;
	my %object = @_;

	print $file_handle, "static $_->{TYPE} $_->{NAME}$_->{ARGS};\n" foreach(@{$object{"PROTOTYPES"}});
}

# read in file, extract class name and super class, extract method names and signatures
sub parse_file {
	my $filename = shift;
	my @types = @_;
	my $object_types = join("|", @types);
	open my $objfile, "<", $filename or die "$!";

	my $multiline_comment;
	my $line;
	do {
		$line = <$objfile>;
		$multiline_comment = (($multiline_comment and $line !~ /^.*\*\/(?!\s*\/\*)/) or $line =~ /\/\*(?!.*\*\/)/);
	} while(($line =~ /^\s*((\/\/.*)|(#.*)|(.*\*\/(?!.*\/\*)))?$/ or $multiline_comment) and !eof $objfile);

	$line =~ /^\s*public\s*class\s*([a-zA-Z_]\w*)\s*implements\s*([a-zA-Z_]\w*)\s*;/ or die "Invalid file $filename...";
	my %class_info = (
		CLASS => $1,
		SUPER => $2
	);
	my @prototypes;

	while(<$objfile>) {
		if($_ =~ /^\s*public\s+((const(\*|\s)+)?(unsigned|((unsigned\s+)?int|short|char|void|(long\s+)?(long|double)|float|$object_types))(\s|\*)+)([a-zA-Z_]\w*)(\([^)]+\))/) {
			my $proto = {
				TYPE => $1,
				NAME => $9,
				ARGS => $10
			};
			push @prototypes, $proto;
		}
	}
	close $objfile;
	$class_info{"PROTOTYPES"} = [ @prototypes ];

	return %class_info;
}

# generate C code for object
sub gen_c_file {
	my $filename = shift;
	my %object = @_;

	open my $objfile, "<", $filename or die "$!";
	$filename =~ s/\.cpp$/.c/;
	open my $gen_code, ">", $filename or die "$!";

	print $gen_code "/* THIS CODE HAS BEEN AUTOMATICALLY GENERATED FROM ${filename}pp.\n",
			"* ALL CHANGES MADE TO THIS FILE WILL BE LOST AT NEXT MAKE INVOCATION */\n\n";

	my $multiline_comment;
	my $line;
	do {
		$line = <$objfile>;
		$multiline_comment = (($multiline_comment and $line !~ /^.*\*\/(?!\s*\/\*)/) or $line =~ /\/\*(?!.*\*\/)/);
		print $gen_code $line if($line =~ /^\s*(#.*)$/);
	} while(($line =~ /^\s*((\/\/.*)|(#.*)|(.*\*\/(?!.*\/\*)))?$/ or $multiline_comment) and !eof $objfile);

	$line =~ /^\s*public\s*class\s*([a-zA-Z_]\w*)\s*implements\s*([a-zA-Z_]\w*)\s*;/ or die "Invalid file ${filename}pp...";
	my $index = 0;

	&prototype_gen($gen_code, %object);
	&LT_gen($gen_code, %object);
	&Builder_gen($gen_code, %object);

	while(<$objfile>) {
		if($_ =~ /^\s*public\s+((const(\*|\s)+)?(unsigned|((unsigned\s+)?int|short|char|void|(long\s+)?(long|double)|float|$object_types))(\s|\*)+)([a-zA-Z_]\w*)(\([^)]+\))/) {
			print $gen_code;
		} else {
			print $gen_code;
		}	
	}

	close $objfile;
	close $gen_code;
}

# Extract all datatypes defined with typedef
sub extract_types {
	my %headers;		# system directories or directories specified with -I flag
	my %local_headers;	# relative path headers
	my $filename = shift;
	my %types = @_;

	open my $file, "<", $filename or die "$!";
	while(<$file>) {
		#	if($_ =~ /^\s*typedef/) {
		#	while($_ !~ /^typedef(.|\n)*
#		push @types, $2 if($_ =~ /^\s*typedef\s+struct\s+([a-zA-Z_]\w*(?!_LT))\s*\*\s*([a-zA-Z_]\w*(?!_LT))\s*;/);
#		$headers{$1} = 1 and &extract_types($1, %) if($_ =~ /\s*#\s*include\s*<(\S+)>\s*$/);
#		$local_headers{$1} = 1 and &extract_types($1, %) if($_ =~ /\s*#\s*include\s*"(\S+)"\s*$/);
	}
	close $file;
	exit 0;
#	my %header_files = (
#		HEADERS => [ @headers ],
#		LOCALS => [ @local_headers ]
#	);
#	return @types;
}

my @includeDirs;
GetOptions("I=s" => \@includeDirs) or die("Error in command line arguments\n");

#my $cpp_output = `\$(cc --print-prog-name=cc1) -v 2>&1 << EOF
#EOF`;
#print $cpp_output;
#exit 0;

foreach(@ARGV) {
	&gen_c_file($_);
	exit 0;
#	my @types = &extract_types(@_); my %header_files = &extract_types($_);
#	print $_, "\n" foreach(@{$header_files{"HEADERS"}});
#	print "lol\n";
#	print $_, "\n" foreach(@{$header_files{"LOCALS"}});
	my @types;
	my %class_info = &parse_file($_, @types);
	print "$_->{NAME}\n" foreach(@{$class_info{"PROTOTYPES"}});
}
