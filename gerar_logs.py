import random

def gerar_sequencial(nome_arquivo, qtd_acessos):
    # Simula a leitura de um grande vetor contínuo na memória (Ex: varredura de vídeo)
    with open(nome_arquivo, 'w') as f:
        endereco = 0x00001000
        for _ in range(qtd_acessos):
            f.write(f"{endereco:08x} R\n")
            endereco += 4 # Incrementa 4 bytes (tamanho de um int padrão)

def gerar_aleatorio(nome_arquivo, qtd_acessos):
    # Simula acessos caóticos sem nenhuma localidade (Ex: tabela hash mal otimizada)
    with open(nome_arquivo, 'w') as f:
        for _ in range(qtd_acessos):
            endereco = random.randint(0, 0x0FFFFFFF)
            op = 'R' if random.random() > 0.2 else 'W' # 80% leitura, 20% escrita
            f.write(f"{endereco:08x} {op}\n")

def gerar_localidade(nome_arquivo, qtd_acessos):
    # Simula um laço de repetição (for/while) travado em um pequeno grupo de variáveis
    with open(nome_arquivo, 'w') as f:
        endereco_base = 0x0000A000
        for _ in range(qtd_acessos):
            # 90% de chance de acessar um endereço muito próximo (mesma página)
            if random.random() > 0.1:
                deslocamento = random.randint(0, 256)
            # 10% de chance de dar um salto para fora da página
            else:
                deslocamento = random.randint(0, 0x000FFFFF)
            
            op = 'R' if random.random() > 0.5 else 'W'
            endereco = endereco_base + deslocamento
            f.write(f"{endereco:08x} {op}\n")

# Gera os arquivos na pasta testes
qtd = 1000000
gerar_sequencial('testes/sequencial.log', qtd)
gerar_aleatorio('testes/aleatorio.log', qtd)
gerar_localidade('testes/localidade.log', qtd)

print("Arquivos gerados com sucesso na pasta 'testes/'.")