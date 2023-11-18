use strict;
use warnings;
use File::Find;
use File::Copy;
use File::Path 'make_path';
use File::Basename;

my $checker_program = "./checker";

# return 0 if the given file exists in given path else return 1
sub search_file
{
	my $path = shift;
	my $i = 5;
	my $file;

	while (substr($path, -$i, 1) ne '/')
	{
		$file = substr($path, -$i, $i);
		$i++;
	}
	$path = "../maps/valids";
	if (-e "$path/$file")
	{
		# print "Ficheiro " . $file . " existe no caminho " . $path . "\n";
		return (1);
	}
	else
	{
		# print "Ficheiro " . $file . " nao existe no caminho " . $path . "\n";
		return (0); # file doesn't exist
	}
}

# replace file number e.g. map_2 become map_3
sub replace_name
{
	my $original_file = shift;
	my $path;
	my $i = 5;
	my $x;
	my $number;

	while (substr($original_file, -$i, 1) ne '/')
	{
		$x = substr($original_file, -$i, $i - 4);
		$i = $i + 1;
	}
	$path = substr($original_file, 0, -$i + 1);

	if ($x =~ /map_(\d+)$/)
	{
    	my $numero = $1;
		$x = substr($x, 0, -(length($numero)));
		$x .= $numero + 1;
	}

	return ("$path$x.ber");
}

sub process_ber_file {
	my $ber_file = $File::Find::name;

	if (-f $ber_file && $ber_file =~ /\.ber$/)
	{
		system('gcc', '-o', 'checker', 'main.c') == 0 or die "Falha na compilação: $!";
		my $return_code = system("$checker_program $ber_file");
		if ($return_code == 0 || $return_code == 139)
		{
			make_path("../maps/valids");
			if (search_file($ber_file) == 0) {
				move($ber_file, "../maps/valids/" . basename($ber_file)) or die "Não foi possível mover $ber_file: $!"; }
			else {
				while (search_file($ber_file) == 1) {
					my $new_name = replace_name($ber_file);
					rename($ber_file, $new_name) or die "Falha ao renomear o arquivo: $!"; 
					$ber_file = $new_name;
				}
				move($ber_file, "../maps/valids/" . basename($ber_file)) or die "Não foi possível mover $ber_file: $!";
			}

			print "Arquivo $ber_file movido para ../maps/valids/.\n";
		}

		if ($return_code == 256) # return == 1
		{
			make_path("../maps/invalids/not_rectangular");
			move($ber_file, "../maps/invalids/not_rectangular/" . basename($ber_file)) or die "Não foi possível mover $ber_file: $!";
			print "Arquivo $ber_file movido para ../maps/invalids/not_rectangular.\n";
		}

		if ($return_code == 512) # return == 2
		{
			make_path("../maps/invalids/toomany_playerORexit");
			move($ber_file, "../maps/invalids/toomany_playerORexit/" . basename($ber_file)) or die "Não foi possível mover $ber_file: $!";
			print "Arquivo $ber_file movido para ../maps/invalids/toomany_playerORexit.\n";
		}

		if ($return_code == 768) # return == 3
		{
			make_path("../maps/invalids/insufficient_columns_lines");
			move($ber_file, "../maps/invalids/insufficient_columns_lines/" . basename($ber_file)) or die "Não foi possível mover $ber_file: $!";
			print "Arquivo $ber_file movido para ../maps/invalids/insufficient_columns_lines.\n";
		}

		if ($return_code == 1024) # return == 4
		{
			make_path("../maps/invalids/wrong_wall");
			move($ber_file, "../maps/invalids/wrong_wall/" . basename($ber_file)) or die "Não foi possível mover $ber_file: $!";
			print "Arquivo $ber_file movido para ../maps/invalids/wrong_wall.\n";
		}

		if ($return_code == 1280) # return == 5
		{
			make_path("../maps/invalids/wrong_character");
			move($ber_file, "../maps/invalids/wrong_character/" . basename($ber_file)) or die "Não foi possível mover $ber_file: $!";
			print "Arquivo $ber_file movido para ../maps/invalids/wrong_character.\n";
		}

		if ($return_code == 1536) # return == 6
		{
			make_path("../maps/invalids/obstructed_path");
			move($ber_file, "../maps/invalids/obstructed_path/" . basename($ber_file)) or die "Não foi possível mover $ber_file: $!";
			print "Arquivo $ber_file movido para ../maps/invalids/obstructed_path.\n";
		}
	}
}

find({ wanted => \&process_ber_file, no_chdir => 1 }, "../maps/unchecked/");

system ('rm', 'checker');