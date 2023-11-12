#!/usr/bin/env python3
# -*- coding: utf-8 -*-
import networkx as nx
import matplotlib.pyplot as plt
import pydot
import sys



def ler_caminhos(file):
    caminhos = []
    with open(file, 'r') as file:
        for line in file:
            caminho = line.strip()
            caminho = line.split("->")
            for i in range(len(caminho)-1):
                caminhos.append((caminho[i], caminho[i+1]))
    return caminhos


def openGraph():
    try:
        with open(file_path, 'r') as file:
            dot_graph = file.read()
            pydot_graph = pydot.graph_from_dot_data(dot_graph)
            G = nx.nx_pydot.from_pydot(pydot_graph[0])
            return G
    except FileNotFoundError:
        print(f"O arquivo '{file_path}' não foi encontrado.")


def plot_path(graph, path):
    G = nx.DiGraph()
    edges = path.split('->')
    
    for i in range(len(edges) - 1):
        G.add_edge(int(edges[i]), int(edges[i + 1]))
    pos = nx.spring_layout(G)
    
    all_edges = list(G.edges())
    print(all_edges)

    non_path_edges = [edge for edge in all_edges if edge not in list(zip(edges[:-1], edges[1:]))]
    print(non_path_edges)
    nx.draw_networkx_nodes(G, pos, node_size=500)
    #nx.draw_networkx_edges(G, pos, edgelist=list(zip(edges[:-1], edges[1:])), edge_color='red', width=2)
    nx.draw_networkx_labels(G, pos)
    
    plt.axis('off')
    plt.show()

edge_weights = {}
ecolors = {}
ncolors = {}

if(len(sys.argv) > 1):
    func = sys.argv[1]
    print("func: %s\n", func)
file_path = '../biowcet/din/dot/'+ func+ '_full.dot'  # Caminho para o arquivo DOT
queen_path = '../biowcet/din/build/path_queen_'+ func + '.txt'
G = openGraph()
p = ler_caminhos(queen_path)

for edge in G.edges(data=True):
    ecolors[(edge[0], edge[1])] = 'black'
    edge_weights[(edge[0], edge[1])] = edge[2]['label']

for edge in G.edges(data=True):
    for i in p:
        if((edge[0].replace(":",""), edge[1].replace(":","")) == i):
            ecolors[(edge[0], edge[1])] = 'red'

for i in G.nodes():
    ncolors[i] = 'skyblue'

G.add_node('Entry', label='entry', shape='box', style='filled', fillcolor='green')
G.add_node('Exit', label='exit', shape='box', style='filled', fillcolor='red')
ncolors['Exit'] = 'red'
ncolors['Entry'] = 'green'

for i in G.nodes():
    if(not G.out_degree(i)):
        G.add_edge(i, 'Exit')
        ecolors[(i, 'Exit')] = 'red'
    elif(not G.in_degree(i)):
        G.add_edge('Entry', i)
        ecolors[('Entry', i)] = 'red'


pos = nx.kamada_kawai_layout(G)
print(ncolors)
nx.draw(G, pos, with_labels=True, node_size=1900, font_weight='bold', width=2, arrows=True, edge_color=[ecolors[edge] for edge in G.edges()], node_color=[ncolors[node] for node in G.nodes()])
nx.draw_networkx_edge_labels(G, pos, edge_labels=edge_weights)
plt.show()
