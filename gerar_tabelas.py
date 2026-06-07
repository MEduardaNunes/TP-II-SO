import pandas as pd

# 1. Carregar os dados
df = pd.read_csv('resultados/resultados_consolidados.csv', sep=';', 
                 names=['Politica', 'Log', 'Memoria', 'Pagina', 'Tabela', 'PageFaults', 'Sujas', 'MemoriaConsumida', 'Acessos'])

# Limpar nome do log (de 'testes/compilador.log' para 'Compilador')
df['Log'] = df['Log'].str.replace('testes/', '').str.replace('.log', '').str.capitalize()

# 2. Gerar dados para a Tabela 1 (Memória Crescente, Pag=4, Tabela Densa/Qualquer)
# Obs: Como page faults no LRU são iguais para todas as tabelas, filtramos por 'Densa' para não duplicar dados.
df_tabela1 = df[(df['Pagina'] == 4) & (df['Tabela'] == 'Densa')]
tabela1 = df_tabela1.pivot_table(index=['Politica', 'Memoria'], columns='Log', values='PageFaults')

# 3. Gerar dados para a Tabela 2 (Pagina Variavel, Mem=4096)
df_tabela2 = df[(df['Memoria'] == 4096) & (df['Tabela'] == 'Densa')]
tabela2 = df_tabela2.pivot_table(index=['Politica', 'Pagina'], columns='Log', values='PageFaults')

# 4. Gerar dados para a Tabela 3 (Estruturas, Pagina Variavel, Mem=4096, Algoritmo fixo, ex: LRU)
df_tabela3 = df[(df['Memoria'] == 4096) & (df['Politica'] == 'LRU') & (df['Log'] == 'Compilador')]
tabela3 = df_tabela3[['Tabela', 'Pagina', 'MemoriaConsumida', 'Acessos']].sort_values(by=['Tabela', 'Pagina'])

df_densa = df_tabela3[df_tabela3['Tabela'] == 'Densa'][['Pagina', 'MemoriaConsumida', 'Acessos']].rename(
    columns={'MemoriaConsumida': 'MemDensa', 'Acessos': 'AcessosDensa'}
)

df_metricas_t3 = df_tabela3.merge(df_densa, on='Pagina')
df_metricas_t3['EconomiaEspaco(x)'] = (df_metricas_t3['MemDensa'] / df_metricas_t3['MemoriaConsumida']).round(2)
df_metricas_t3['CustoAcesso(x)'] = (df_metricas_t3['Acessos'] / df_metricas_t3['AcessosDensa']).round(2)
tab3_metricas = df_metricas_t3[['Tabela', 'Pagina', 'EconomiaEspaco(x)', 'CustoAcesso(x)']].sort_values(by=['Tabela', 'Pagina'])

total_linhas_logs = {
    'Compilador': 1000000, 
    'Matriz': 1000000, 
    'Compressor': 1000000, 
    'Simulador': 1000000
}

df['TotalRefs'] = df['Log'].map(total_linhas_logs)
df['TaxaFalha(%)'] = (df['PageFaults'] / df['TotalRefs'] * 100).round(2)
df['TaxaSujas(%)'] = df.apply(
    lambda row: (row['Sujas'] / row['PageFaults'] * 100) if row['PageFaults'] > 0 else 0, 
    axis=1
).round(2)

# Filtrar o recorte para TODAS as políticas (mantendo a página de 4KB como base de comparação)
df_texto = df[(df['Tabela'] == 'Densa') & (df['Pagina'] == 4)].copy()
df_texto = df_texto.sort_values(by=['Log', 'Memoria', 'Politica'])

caminho_saida = 'resultados/tabelas_latex.txt'

with open(caminho_saida, 'w', encoding='utf-8') as f:
    f.write("="*60 + "\nTABELA 1 LATEX: Comportamento com Memória Crescente (4 KB)\n" + "="*60 + "\n")
    f.write(tabela1.to_string() + "\n\n")
    
    f.write("="*60 + "\nTABELA 2 LATEX: Comportamento com Página Variável (Memória 4 MB)\n" + "="*60 + "\n")
    f.write(tabela2.to_string() + "\n\n")
    
    f.write("="*60 + "\nTABELA 3 LATEX: Custo das Estruturas (Compilador, LRU, Memória 4 MB)\n" + "="*60 + "\n")
    f.write(tabela3.to_string(index=False) + "\n\n")
    
    f.write("="*60 + "\nMÉTRICAS RELATIVAS: Para análise da Tabela 3 no texto\n" + "="*80 + "\n")
    f.write(tab3_metricas.to_string(index=False) + "\n\n")
    
    f.write("="*60 + "\nMÉTRICAS DE FALTAS/SUJAS: Para análise das Tabelas 1 e 2 no texto\n" + "="*60 + "\n")
    f.write(df_texto[['Politica', 'Log', 'Memoria', 'PageFaults', 'TaxaFalha(%)', 'TaxaSujas(%)']].to_string(index=False) + "\n")

print(f"Sucesso! Abra o arquivo '{caminho_saida}' para copiar os dados")
