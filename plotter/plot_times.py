#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import pandas as pd
import matplotlib.pyplot as plt
path = "./times/"
# Carrega o arquivo CSV
df = pd.read_csv(path +'output.csv')

# Calcula as médias
avg_cyclesI = df['cyclesI'].mean()
avg_cyclesB = df['cyclesB'].mean()
avg_ms_ipet = df['ms_ipet'].mean()
avg_ms_bio = df['ms_bio'].mean()

# Cria os gráficos
fig, (ax1, ax2) = plt.subplots(2, 1, figsize=(10, 8))

# Gráfico para WCETs (ciclos)
ax1.plot(df['Execution'], df['cyclesB'], marker='o', label='Ciclos ACO')
ax1.axhline(y=avg_cyclesB, color='b', linestyle='--', label=f'Média Ciclos ({avg_cyclesB:.0f})')
ax1.plot(df['Execution'], df['cyclesI'], marker='o', label='Ciclos IPET')
ax1.axhline(y=avg_cyclesI, color='r', linestyle='--', label=f'Média Ciclos ({avg_cyclesI:.0f})')
ax1.set_ylabel('Ciclos')
ax1.set_title('WCETs e Tempo de Execução do BubbleSort')

# Gráfico para tempos de execução (ms)
ax2.plot(df['Execution'], df['ms_ipet'], marker='o', label='IPET')
ax2.plot(df['Execution'], df['ms_bio'], marker='o', label='BIO')
ax2.axhline(y=avg_ms_ipet, color='b', linestyle='--', label=f'Média IPET ({avg_ms_ipet:.5f})')
ax2.axhline(y=avg_ms_bio, color='r', linestyle='--', label=f'Média BIO ({avg_ms_bio:.5f})')
ax2.set_xlabel('Execução')
ax2.set_ylabel('Millisegundos')

# Adiciona legenda
ax1.legend()
ax2.legend()
plt.savefig('times_wcet_plot.pdf', format='pdf')
# Exibe os gráficos
plt.tight_layout()
plt.show()
