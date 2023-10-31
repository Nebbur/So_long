use strict;
use warnings;
use List::Util qw(shuffle);
use File::Glob ':bsd_glob';
use File::Path 'make_path';

my $file_path;

# Defina o número de arquivos que deseja gerar
my $num_files = 10;

for my $file_num (1..$num_files)
{
	my $output_file = "map_$file_num.ber";  # Nome do arquivo de saída
	my $output_directory;
	# Abra o arquivo de saída para gravação
	open my $fh, '>', $output_file or die "Não foi possível abrir o arquivo $output_file: $!";

	# Defina os símbolos desejados, incluindo '0'
	my @symbols = ('1', '0', 'C');  # Remova 'P' e 'E'

	# Embaralhe a lista de símbolos
	@symbols = shuffle(@symbols);

	# Gere tamanhos de linha e coluna aleatórios
	my $rows = int(rand(13)) + 5; # Pelo menos 5 linhas
	my $cols = int(rand(14)) + 3; # Pelo menos 3 colunas

	# Calcule o número equilibrado de collectables com base no tamanho do mapa
	my $collectables_count = int(($rows * $cols) / 15);  # Ajuste o valor 15 conforme necessário

	# Certifique-se de que um 'P' e um 'E' sejam gerados aleatoriamente
	my $player_row = int(rand($rows - 2)) + 2;  # Garanta que o 'P' não seja gerado nas bordas
	my $player_col = int(rand($cols - 2)) + 2;
	my $exit_row = int(rand($rows - 2)) + 2;  # Garanta que o 'E' não seja gerado nas bordas
	my $exit_col = int(rand($cols - 2)) + 2;

	my $obstacle_counter = 0;  # Contador de obstáculos gerados
	my $collectables_counter = 0;  # Contador de collectables gerados

	# Gere um mapa aleatório com os símbolos embaralhados
	for my $row (1..$rows) {
		for my $col (1..$cols) {
			my $symbol;
			if ($row == 1 || $row == $rows || $col == 1 || $col == $cols) {
				$symbol = '1';  # A primeira e última linha e coluna têm apenas '1'
			} elsif ($row == $player_row && $col == $player_col) {
				$symbol = 'P';  # Gere o 'P' na posição aleatória
			} elsif ($row == $exit_row && $col == $exit_col) {
				$symbol = 'E';  # Gere o 'E' na posição aleatória
			} else {
				if ($obstacle_counter < 10 && int(rand(4)) == 0) {
					$symbol = '1';  # Aumente a probabilidade de gerar '1' no início
					$obstacle_counter++;
				} else {
					if ($collectables_counter < $collectables_count && int(rand(3)) == 0) {
						$symbol = 'C';  # Gere um collectable 'C'
						$collectables_counter++;
					} else {
						# Escolha aleatoriamente entre os símbolos restantes, excluindo '1' e 'C'
						my @valid_symbols = grep { $_ ne '1' && $_ ne 'C' } @symbols;
						$symbol = $valid_symbols[rand @valid_symbols];
					}
				}
			}
			print $fh $symbol;  # Redirecione a saída para o arquivo
		}
		print $fh "\n";  # Inclua uma quebra de linha no arquivo
	}

	make_path("../maps/unchecked");
	# Novo diretorio desejado
	if ($collectables_counter == 0)
	{
		$output_directory = "../maps/unchecked/0_coin";
	}
	if ($collectables_counter == 1)
	{
		$output_directory = "../maps/unchecked/1_coin";
	}
	if ($collectables_counter == 2)
	{
		$output_directory = "../maps/unchecked/2_coin";
	}
	if ($collectables_counter == 3)
	{
		$output_directory = "../maps/unchecked/3_coin";
	}
	if ($collectables_counter == 4)
	{
		$output_directory = "../maps/unchecked/4_coin";
	}
	if ($collectables_counter == 5)
	{
		$output_directory = "../maps/unchecked/5_coin";
	}
	if ($collectables_counter == 6)
	{
		$output_directory = "../maps/unchecked/6_coin";
	}
	if ($collectables_counter >= 7)
	{
		$output_directory = "../maps/unchecked/7plus_coin";
	}

	# Crie o diretório se ele não existir
	make_path($output_directory);

	# Listar todos os arquivos no diretório
	my @arquivos = bsd_glob("$output_directory/*");

	# Contar o número de arquivos
	my $quantidade_arquivos = scalar @arquivos + 1;

	my $new_output_file = "$output_directory/map_$quantidade_arquivos.ber";

	# Renomeie o arquivo
	rename($output_file, $new_output_file) or die "Falha ao renomear o arquivo: $!";

	# Feche o arquivo de saída
	close $fh;

	print "Mapa $quantidade_arquivos gerado e salvo em $output_directory com $collectables_counter coletaveis.\n";
}
	system ('perl', 'checker.pl');
	system ('rm', '-rf', '../maps/unchecked/');
