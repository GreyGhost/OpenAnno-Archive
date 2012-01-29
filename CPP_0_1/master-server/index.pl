#!/usr/bin/perl -T
use strict;
use warnings;
use XML::Smart;
use CGI::Carp qw/ fatalsToBrowser /;
use HTML::Template::Expr;
use DBI;
use POSIX qw/ strftime locale_h /;
setlocale(LC_ALL, "de_DE.UTF-8");


my $CONFIGFILE = "/usr/local/etc/openanno/config.xml";

my $XML = XML::Smart->new($CONFIGFILE);
$XML = $XML->cut_root;
my $dbhost = $XML->{"database"}{"host"}[0];
my $dbuser = $XML->{"database"}{"user"}[0];
my $dbpass = $XML->{"database"}{"password"}[0];
my $db     = $XML->{"database"}{"database"}[0];
my $dbport = $XML->{"database"}{"port"}[0];
$dbport = 3306 unless ("$dbport");
my $dbh    = DBI->connect("dbi:mysql:database=$db;host=$dbhost;port=$dbport", "$dbuser", "$dbpass", { RaiseError => 1 } ) or die $DBI::errstr;
my $query  = "SELECT location,name,host,ping FROM games ORDER BY ping";
my $sth    = $dbh->prepare($query);
$sth->execute;
my @tablerows = ();
while (my @server = $sth->fetchrow_array()) {
	my %row = (
			location => $server[0],
			name     => $server[1],
			ip       => $server[2]
	          );
	push @tablerows, \%row;
}
my $template = HTML::Template::Expr->new(filename => "list.tmpl");
$template->param(list_loop => \@tablerows);
$template->param(time      => strftime "%a %b %e %H:%M:%S %Y", localtime);

print "Content-Type: text/html\n\n";
print $template->output;
