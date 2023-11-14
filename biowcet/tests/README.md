Instruções para rodar os testes:
===========
Ambiente do OTAWA instalado e configurado;
GoogleTest instalado.
Como instalar o googleTest:
---------------------------------------------------
    git clone https://github.com/google/googletest.git -b v1.14.0
    cd googletest
    mkdir build
    cd build
    cmake ..
    instalar: make
    instalar para os adm do sistema: make install
    ou
    apt install libgtest-dev
Executar comando make para rodar o makefile.

É necessário rodar pelo menos uma vez uma análise do BioWcet, já que os arquivos de teste estão "mockados".
---------------------------------------------------

./unit_test
---------------------------------------------------