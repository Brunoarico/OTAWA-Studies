# Estudos sobre o software Otawa

Criamos uma aplicação de exemplo. Cuja organização é:
```bash
├── Makefile
├── ipet.c
├── msgLogger.c
├── msgLogger.h
├── hardware:
    ├── memory.xml
    ├── processor.xml
├── test_files
    ├── bs.elf
```

Na raíz temos os arquivos principais do processo de compilação, em hardware temos os arquivos de descrição do processador e da memória, e em test_files temos o arquivo binário que será analisado.

## Compilação

Para compilar utilize o comando:
```bash
make
```

## Execução
Para executar utilize o comando:
```bash
make run
```
E ele executara o programa com a entrada sendo o arquivo `bs.elf` que está em test_files. Este arquivo é o binario correspondente a uma busca binária.

# Makefile

É usado para construir o programa `ipet`. O programa é destinado a ser executado com arquivos de teste específicos localizados na pasta `test_files` e requer uma pasta `hardware`. Abaixo, você encontrará uma explicação dos diferentes componentes e comandos no Makefile.

## Programa

- `ipet`: O programa principal que será construído usando este Makefile.

## Fontes

Os arquivos-fonte para o programa `ipet` são os seguintes:

- `ipet.cpp`: O arquivo-fonte principal para o programa `ipet`.
- `msgLogger.cpp`: Um arquivo-fonte contendo funcionalidade de registro de mensagens.

## Configuração

O Makefile usa as seguintes configurações para construir o programa `ipet`:

- `FLAGS`: Flags do compilador obtidas a partir de `otawa-config --cflags`.
- `DATADIR`: Diretório de dados obtido a partir de `otawa-config --datadir`.
- `CXXFLAGS`: Flags do compilador incluindo o caminho do diretório de dados e suporte ao padrão `C++11`.
- `LDLIBS`: Flags de biblioteca obtidas a partir de `otawa-config --libs`.
- `CFLAGS`: Flags do compilador para suporte ao padrão `C++11`.
- `LDFLAGS`: Flags para carregar bibliotecas compartilhadas usando `dlopen`.
- `CXX`: O compilador C++ a ser usado (`g++` neste caso).
- `LD`: O comando de ligação usado para construir o programa `ipet`.
- `LD_LIBRARY_PATH`: Caminho da biblioteca obtido a partir de `otawa-config --libdir`.

## Derivados

- `OBJECTS`: Arquivos de objeto a serem criados a partir dos arquivos-fonte.
- `DEPS`: Arquivos de dependência usados para rastreamento automático de dependências durante o processo de construção.

## Regras

O Makefile define as seguintes regras para construir o programa `ipet`:

- `all`: A regra padrão que constrói a pasta `.deps` e o programa `ipet`. Também define a variável de ambiente `LD_LIBRARY_PATH`, mas tenha em mente que ela não será permanente para a sessão.
- `$(PROGRAM)`: Regra para construir o programa `ipet` usando os arquivos de objeto compilados.
- `.deps`: Regra para criar a pasta `.deps` se ela não existir.
- `%.o`: Regra para compilar cada arquivo-fonte em arquivos de objeto, gerando também arquivos de dependência.
- `-include $(DEPS)`: Inclui os arquivos de dependência gerados no Makefile para garantir um rastreamento de dependências atualizado.
- `clean`: Regra para remover todos os arquivos de objeto construídos, o programa `ipet`, arquivos de backup e outros artefatos temporários.
- `run`: Regra para executar o programa `ipet` com parâmetros específicos (TEST_TARGET e HARDWRE_FOLDER).


# Necessidade de export

A execução do binario gerado pode necessitar de que haja uma referência para a biblioteca dinamica do Otawa `libotawa.so` no `LD_LIBRARY_PATH`. Para isso, basta executar o comando:
```bash
    LD_LIBRARY_PATH=$(otawa-config --libdir)
    export LD_LIBRARY_PATH
    make run
```
E o programa executaara normalmente.

# ToDo

- No estado atual o programa não esta conseguindo fazer a leitura do CFG, e deveria



