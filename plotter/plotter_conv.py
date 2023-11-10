import matplotlib.pyplot as plt
import sys

def plotar_grafico(nome_arquivo):
    with open(nome_arquivo, 'r') as arquivo:
        # Lendo os valores do arquivo e removendo espaços em branco e quebras de linha
        valores = [int(line.strip()) for line in arquivo.readlines()]

        # Plotando os valores em um gráfico de linha
        plt.plot(valores)
        plt.title('WCETS')
        plt.xlabel('ants')
        plt.ylabel('wcet parcial')
        plt.grid(True)
        plt.show()

# Chamando a função com o nome do arquivo que contém os valores
if(len(sys.argv) > 1):
    func = sys.argv[1]
path_wcet = '../biowcet/din/build/wcets_'+ func + '.txt'  # Caminho para o arquivo DOT
plotar_grafico(path_wcet)
