#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import networkx as nx
import pydot
import matplotlib.pyplot as plt
import matplotlib.patches as patches
from matplotlib.colors import Normalize
from matplotlib.cm import ScalarMappable
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
        print("equal")
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
ecolors = {}
ncolors = {}
emax = max(resultado.values())
emin = min(resultado.values())
for edge in G.edges(data=True):
    #print(edge)
    edge_weights[(edge[0], edge[1])] = edge[2]['label']
    if((edge[0], edge[1]) not in resultado):
        ecolors[(edge[0], edge[1])] = 'black'
    else:
        ecolors[(edge[0], edge[1])] = gerar_cores_gradiente(resultado[(edge[0], edge[1])], emax, emin)
        #print(colors[(edge[0], edge[1])], resultado[(edge[0], edge[1])], emax, emin)

G.add_node('Entry', label='entry', shape='box', style='filled', fillcolor='green')
G.add_node('Exit', label='exit', shape='box', style='filled', fillcolor='red')
for i in G.nodes():
    ncolors[i] = 'skyblue'
    i = i.replace(':', '')
ncolors['Exit'] = 'red'
ncolors['Entry'] = 'green'

for i in G.nodes():
    if(not G.out_degree(i)):
        G.add_edge(i, 'Exit', colors = 'black', label='0')
        ecolors[(i, 'Exit')] = 'black'
    elif(not G.in_degree(i)):
        G.add_edge('Entry', i, colors = 'black', label='0')
        ecolors[('Entry', i)] = 'black'


pos = nx.kamada_kawai_layout(G)

f = plt.figure(1)

ax = f.add_subplot(1,1,1)

norm = Normalize(vmin=min(resultado.values()), vmax=max(resultado.values()))
cmap = plt.get_cmap('magma_r')
sm = ScalarMappable(cmap=cmap, norm=norm)
print (list(resultado.values()))
sm.set_array(list(resultado.values()) +[0])  # Define uma lista vazia, mas poderia ser uma lista de valores para a escala de cor

# Adiciona a legenda ao subplot com a gradação de cor
plt.colorbar(sm, label='N° de passagens')

handles_dict = {patches.Patch(color='white', label=f'{edge[0]} -> {edge[1]} = {value}') for edge, value in resultado.items()}

#ax.legend(handles=handles_dict,title='N° de Passagens')
nx.draw(G, pos, with_labels=True, node_size=1900, font_weight='bold', edge_labels=edge_weights, width=2, arrows=True, edge_color=[ecolors[edge] for edge in G.edges()], node_color=[ncolors[node] for node in G.nodes()], ax=ax)
nx.draw_networkx_edge_labels(G, pos, edge_labels=edge_weights, ax=ax)
figure = plt.gcf()
figure.set_size_inches(10, 6)
plt.savefig('path.pdf', format='pdf')
plt.show()
