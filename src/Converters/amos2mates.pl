#!/usr/local/bin/perl

use TIGR::AmosLib;
use TIGR::Foundation;
use strict;

my $VERSION = '$Revision$ ';
my $HELP = q~
    amos2mates [-i infile] [-o outfile] 

    if -i and -o are not provided reads from STDIN and writes to STDOUT
    if -i is provided but -o is not, outfile is same as infile except for the
  extension
    otherwise -i and -o are those specified in the command line
    if -i is provided the filename must end in .afg
~;

my $base = new TIGR::Foundation();
if (! defined $base) {
    die("A horrible death\n");
}


$base->setVersionInfo($VERSION);
$base->setHelpInfo($HELP);

my $infile;
my $outfile;
my $inname = "stdin";
my $accession = 1;
my $date = time();
my $firstunv = 1;
my %libids;
my %frg2lib;
my %rd2lib;
my %rdids;

my $err = $base->TIGR_GetOptions("i=s"   => \$infile,
				 "o=s"   => \$outfile);



# if infile is provided, make sure it ends in .afg and open it
if (defined $infile){
    if ($infile !~ /\.afg$/){
	$base->bail ("Input file name must end in .afg");
    }
    $inname = $infile;
    open(STDIN, $infile) || $base->bail("Cannot open $infile: $!\n");
}

# if infile is provided but outfile isn't make outfile by changing the extension
if (! defined $outfile && defined $infile){
    $outfile = $infile;
    $outfile =~ s/(.*)\.afg$/\1.mates/;
}

# if outfile is provided (or computed above) simply open it
if (defined $outfile){
    open(STDOUT, ">$outfile") || $base->bail("Cannot open $outfile: $!\n");
}

while (my $record = getRecord(\*STDIN)){
    my ($type, $fields, $recs) = parseRecord($record);

    if ($type eq "LIB"){
	# only use the first DST record  
	if ($#$recs < 0){
	    $base->bail("LIB record doesn't have any DST record at or around line $. in input");
	}
        my ($sid, $sfs, $srecs) = parseRecord($$recs[0]);
	if ($sid ne "DST"){
	    $base->bail("LIB record doesn't start with DST record at or around line $. in input");
	}
	my $min = int($$sfs{mea} - 3 * $$sfs{std});
	my $max = int($$sfs{mea} + 3 * $$sfs{std});
	$min = 0 if $min < 0;
	
	print "library\t$$fields{eid}\t$min\t$max\n";
    } # type is LIB
    
    if ($type eq "FRG"){
	$frg2lib{$$fields{"eid"}} = $$fields{"lib"};
    } # type is FRG

    if ($type eq "RED"){
	$rd2lib{$$fields{"eid"}} = $frg2lib{$$fields{"frg"}};
	
	my @lines;
	@lines = split('\n', $$fields{com});
	my $seqname = join('', @lines);
	if ($seqname =~ /^\s*$/){
	    $seqname = $$fields{eid};
	}

	$rdids{$$fields{"eid"}} = $seqname;
    } # type is RED
    if ($type eq "MTP"){
	
	if ($rd2lib{$$fields{rd1}} != $rd2lib{$$fields{rd2}}){
	    $base->bail("Reads $$fields{rd1} and $$fields{rd2} don't appear to map to the same library ($rd2lib{$$fields{rd1}} != $rd2lib{$$fields{rd2}})");
	}
	print "$rdids{$$fields{rd1}}\t$rdids{$$fields{rd2}}\t$rd2lib{$$fields{rd1}}\n";
    } # type is MTP
} # while each record

exit(0);
