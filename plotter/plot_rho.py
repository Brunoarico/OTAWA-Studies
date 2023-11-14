#!/usr/bin/env python3
# -*- coding: utf-8 -*-
import pandas as pd
import matplotlib.pyplot as plt
import sys
basepath="./rho/"

def calcular_media(dados):
    return dados.mean(axis=1)

if len(sys.argv) > 1:
    files = sys.argv[1]

arquivos_csv= ['bubbleSort10,02.txt', 'bubbleSort10,05.txt', 'bubbleSort10,09.txt']


dfs = []


now = arquivos_csv


for arquivo in now:
    df = pd.read_csv(basepath+arquivo,  header=None, names=['y'])
    dfs.append(df)


plt.figure(figsize=(10, 6))

# Plota os dados dos CSVs
for i, df in enumerate(dfs):
    plt.plot(df.index, df['y'], label=now[i].replace('.txt', '').replace('bubbleSort','').replace(',09', '0.9').replace(',05', '0.5').replace(',02', '0.2').replace('10','') + " rho")

# Calcula a média dos valores dos CSVs
media = pd.concat([df['y'] for df in dfs], axis=1).mean(axis=1)

# Plota a curva da média
#plt.plot(df.index, media, label='Média', linestyle='--', linewidth=2)

# Adiciona legendas e rótulos aos eixos
plt.legend()
plt.xlabel('Iterações')
plt.ylabel('WCET Rainha')


plt.title('BubbleSort Variando Rhp')

# Exibe o gráfico
plt.savefig('bubbleSort rho.pdf', format='pdf')
plt.show()
