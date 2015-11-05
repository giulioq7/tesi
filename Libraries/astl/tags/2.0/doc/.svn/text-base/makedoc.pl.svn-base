#!/usr/bin/perl

my $output;
for $input (@ARGV) {

    open INPUTFILE, "<".$input || die;
    while (<INPUTFILE>) {
	if (/^\/\*\* *algorithm/) {
	    my %doc = ( "category" => [ "algorithm" ] );
	    while (<INPUTFILE>) {
		last if /^ *\*\/ *$/;
		next if /^ *$/;
		/^ *([a-z]+) +(.+)$/;
		my $command = $1;
		my $stop = $2;
		if ($stop =~ /^.$/) { # multi line argument ?
		    while (<INPUTFILE>) {
			chop $_;
			last if ($_ eq "$stop");
			push @{ $doc{$command} }, $_;
		    }
		}
		else { push @{ $doc{$command} }, $stop; }
	    }

	    if (scalar(%doc)) {
		my $template;
		my $filename = $doc{"category"}[0] . ".tex";
		open TEMPLATE_FILE, "<$filename" || die;
		my $line;
		while ($line = <TEMPLATE_FILE>) {
		    $template .= $line;
		}
		close TEMPLATE_FILE;
		if (!$doc{"definition"}) {
		    $input =~ s/.*\///;
		    push @{$doc{"definition"}}, $input;
		}
		for $section (keys %doc) {
		    if ($section eq "requirement" ||
			$section eq "precondition" ||
			$section eq "note") {
			for $i (0 .. scalar($#{ $doc{$section} })) {
			    $template =~ s/%$section%/$doc{$section}[$i]/;
			}
			
		    }
		    else {
			my $value = $doc{$section}[0];
			for $i (1 .. $#{ $doc{$section} }) {
			    $value .= "\n" . $doc{$section}[$i];
			}

			if ($value) {
			    if ($section eq "name") { $value =~ s/_/\\_/g; }
			    while ($template =~ s/%$section%/$value/) { };
			    if (0) {
#			    if ($section eq "example") {
				print STDERR "Compiling example for $doc{name}[0]... ";
				`mkdir -p examples` && die;
				my $code_sample = "examples/" . $doc{name}[0] . ".cpp";
				open (EXAMPLE_FILE, ">$code_sample");
				print EXAMPLE_FILE "$value\n";
				close EXAMPLE_FILE;
				system("g++ -Wall -fno-rtti -fno-exceptions -I../include $code_sample -o /dev/null")
				    && die ("\n*** Unable to compile example for $doc{name}[0]:/\n" . `cat -n $code_sample` . "\n");
				print STDERR "OK\n";
			    }
			}
		    }
		}
		# enleve les substitution non-utilisées:
		while ($template =~ s/\r?\n[^\n]*%[a-z]+%[^\n]*\n/\n/) {};
		# enleve les listes vides:
		while ($template =~ s/\r?\n\\begin\{itemize\} *\r?\n\\end\{itemize\} *//) {};
		# enleve les paragraphes vides:
		while ($template =~ s/\r?\n\\paragraph\{[^\r\n]*\} *\r?(\n\\paragraph\{[^\r\n]*\})/$1/) {};
		$output .= $template;
		%doc = ();
	    }
	    
	}
    }
    close INPUTFILE;
}
print $output;

