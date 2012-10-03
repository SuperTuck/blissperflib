#!/usr/bin/perl -w
$path = "./bliss/dayz_server/compile";
opendir(DIR, $path);
@files = grep(/\.sqf$/, readdir(DIR));
closedir(DIR);

open(INIFILE, ">blissperflib.ini");
print INIFILE "[profiler]\n";
$i = 10;
foreach $file (@files) {
	open(SQFFILE, "<$path/$file");
	@lines = <SQFFILE>;
	close(SQFFILE);

	open(SQFFILE, ">$path/$file");
	$line = sprintf("_PCID%02d = \"blissperflib\" callExtension \"I:%02d\";\n\n", $i, $i);
	print SQFFILE "$line"; 
	foreach $line (@lines)
	{
		print SQFFILE"$line";
	}

	$line = sprintf("\n\"blissperflib\" callExtension format[\"O:%%1\",_PCID%02d];\n", $i);
	print SQFFILE "$line";
	close(SQFFILE);

	my($name,$ext) = split(/\.sqf/, $file);
	$line = sprintf("%02d = %s\n", $i, $name);
	print INIFILE "$line";
	$i = $i + 1;
}
close(INIFILE);
