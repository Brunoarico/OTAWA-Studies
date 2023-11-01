#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import networkx as nx
import pydot
import matplotlib.pyplot as plt
import numpy as np
import sys
#func = 'bubbleSort'
func = 'main'


if(len(sys.argv) > 1):
    func = sys.argv[1]

file_path = '../biowcet/din/dot/'+ func+ '_full.dot'  # Caminho para o arquivo DOT
path_path = '../biowcet/din/build/paths_'+ func + '.txt'  # Caminho para o arquivo DOT

def ler_caminhos():
    caminhos = []

    with open(path_path, 'r') as file:
        for line in file:
            caminho = line.strip()  # Remove espaços em branco e quebras de linha
            caminhos.append(caminho)

    return caminhos


def contar_passagens(sequencia_caminhos):
    # Inicializa um dicionário para contar a frequência de passagens em cada aresta
    frequencia_arestas = dict()

    # Itera sobre os caminhos e conta as arestas
    for caminho in sequencia_caminhos:
        arestas = caminho.split('->')  # Divide o caminho para obter as arestas
        for i in range(len(arestas) - 1):
            aresta = (arestas[i].strip()+':', arestas[i + 1].strip()+':') # Obtém a aresta (par de nós)
            if(aresta not in frequencia_arestas):
                frequencia_arestas[aresta] = 1
            else:
                frequencia_arestas[aresta] += 1  # Incrementa a contagem de passagens nessa aresta

    return frequencia_arestas

def gerar_cores_gradiente(valor, valor_maximo, valor_minimo):
    if(valor_maximo == valor_minimo):
        return 'black'
    # Normaliza o valor dentro da faixa de 0 a 1
    valor = np.log(valor + 1)
    valor_maximo = np.log(valor_maximo + 1)
    valor_minimo = np.log(valor_minimo + 1)
    normalized_value = (valor - valor_minimo) / (valor_maximo - valor_minimo)

    # Use o colormap 'RdYlGn' (vermelho-amarelo-verde) para criar o gradiente de cores
    cmap = plt.get_cmap('magma_r')

    # Obtenha a cor correspondente ao valor normalizado
    color = cmap(normalized_value)

    # Retorna a cor em formato RGBA
    return color

caminhos =ler_caminhos()
resultado = contar_passagens(caminhos)
#print(resultado) 

try:
    with open(file_path, 'r') as file:
        dot_graph = file.read()
except FileNotFoundError:
    print(f"O arquivo '{file_path}' não foi encontrado.")

pydot_graph = pydot.graph_from_dot_data(dot_graph)
G = nx.nx_pydot.from_pydot(pydot_graph[0])

edge_weights = {}
colors = {}
emax = max(resultado.values())
emin = min(resultado.values())
for edge in G.edges(data=True):
    #print(edge)
    edge_weights[(edge[0], edge[1])] = edge[2]['label']
    if((edge[0], edge[1]) not in resultado):
        colors[(edge[0], edge[1])] = 'white'
    else:
        colors[(edge[0], edge[1])] = gerar_cores_gradiente(resultado[(edge[0], edge[1])], emax, emin)
        #print(colors[(edge[0], edge[1])], resultado[(edge[0], edge[1])], emax, emin)

pos = nx.kamada_kawai_layout(G)
nx.draw(G, pos, with_labels=True, node_size=500, node_color='skyblue', font_weight='bold', edge_labels=edge_weights, width=2, arrows=True, edge_color=[colors[edge] for edge in G.edges()])
nx.draw_networkx_edge_labels(G, pos, edge_labels=edge_weights)
plt.title('Grafo lido do arquivo .dot usando NetworkX')
legend_elements = [
    plt.Line2D([0], [0], marker='o', color='w', markerfacecolor='red', markersize=10, label='Valor 1'),
    plt.Line2D([0], [0], marker='o', color='w', markerfacecolor='green', markersize=10, label='Valor 2'),
    plt.Line2D([0], [0], marker='o', color='w', markerfacecolor='blue', markersize=10, label='Valor 3')
]

# Adicionar a legenda
plt.legend(handles=legend_elements, title='Legenda', loc='upper left', bbox_to_anchor=(1, 1))

plt.show()
