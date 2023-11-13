#!/usr/bin/env python3
# -*- coding: utf-8 -*-
import pandas as pd
import matplotlib.pyplot as plt
import sys
basepath="./wcets200/"

def calcular_media(dados):
    return dados.mean(axis=1)

if len(sys.argv) > 1:
    files = sys.argv[1]

arquivos_csv09 = ['bubbleSort1,09.txt', 'bubbleSort5,09.txt', 'bubbleSort10,09.txt']
arquivos_csv05 = ['bubbleSort1,05.txt', 'bubbleSort5,05.txt', 'bubbleSort10,05.txt', 'bubbleSort15,05.txt']
arquivos_csv02 = ['bubbleSort1,02.txt', 'bubbleSort5,02.txt', 'bubbleSort10,02.txt']

dfs = []

if files == '0.9':
    now = arquivos_csv09
elif files == '0.5':
    now = arquivos_csv05
else:
    now = arquivos_csv02

for arquivo in now:
    df = pd.read_csv(basepath+arquivo,  header=None, names=['y'])
    dfs.append(df)


plt.figure(figsize=(10, 6))

# Plota os dados dos CSVs
for i, df in enumerate(dfs):
    plt.plot(df.index, df['y'], label=now[i].replace('.txt', '').replace('bubbleSort','').replace(',09', '').replace(',05', '').replace(',02', '') + " formigas")

# Calcula a média dos valores dos CSVs
media = pd.concat([df['y'] for df in dfs], axis=1).mean(axis=1)

# Plota a curva da média
plt.plot(df.index, media, label='Média', linestyle='--', linewidth=2)

# Adiciona legendas e rótulos aos eixos
plt.legend()
plt.xlabel('Iterações')
plt.ylabel('WCET Rainha')

if(now == arquivos_csv05):
    plt.title('BubbleSort rho=0.5')
elif(now == arquivos_csv02):
    plt.title('BubbleSort rho=0.2')
else:
    plt.title('BubbleSort rho=0.9')

# Exibe o gráfico
plt.savefig('bubbleSort'+ files +'.pdf', format='pdf')
plt.show()
