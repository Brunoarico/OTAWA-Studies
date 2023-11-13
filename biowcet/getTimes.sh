csv_file="../plotter/times/output.csv"

# Cria o cabeçalho do arquivo CSV
echo "Execution,cyclesB,cyclesI,ms_ipet,ms_bio" > "$csv_file"

# Loop para 10 execuções
for ((i=1; i<=10; i++)); do
    # Executa o comando e armazena a saída
    output=$(./wcet -s m3 -p test_files/bsort100/main.c)

    # Extrai os valores de cycles e ms usando grep e awk
    cyclesI=$(echo "$output" | grep -oP 'WCET_IPET\[main\] = \K\d+')
    cyclesB=$(echo "$output" | grep -oP 'WCET_BIO\[main\] = \K\d+')
    ms_ipet=$(echo "$output" | grep -oP 'IPET consumed \K[0-9.]+')
    ms_bio=$(echo "$output" | grep -oP 'BIO consumed \K[0-9.]+')

    # Adiciona os valores ao arquivo CSV
    echo "$i,$cyclesB,$cyclesI,$ms_ipet,$ms_bio" >> "$csv_file"
done

echo "CSV criado com sucesso em $csv_file!"