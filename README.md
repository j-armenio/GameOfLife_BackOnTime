GameOfLife_BackOnTime

João Armênio Silveira
Fábio Naconeczny da Silva

Esse trabalho consiste na implementação de uma forma de retornar a um estado anterior do famoso "Conway's Game of Life". Seguem os detalhes da implementação.

A abordagem implementada usa técnicas baseadas em SAT e MaxSAT para explorar e validar estados possíveis que atendam às restrições do jogo.

O programa opera em duas fases principais:

    Geração de cláusulas: transforma o problema do Game of Life em uma formulação lógica no formato DIMACS/Weighted CNF. Cláusulas baseadas no artigo "Time-Reversal in Conway’s Life as SAT. Stuart Bain. 2007 Arquivo" adaptado para MaxSAT.
    Resolução via MaxSAT: Usa o SAT solver Open-WBO para encontrar soluções que satisfaçam as restrições e minimizem o número de células vivas.

Estratégia implementada:

Geração de cláusulas:

    Cada célula da matriz é representada por uma variável única.
    As regras do Game of Life (loneliness, stagnation, overcrowding, preservation e life) são mapeadas para cláusulas lógicas por meio de recursões que agrupam e adicionam cada subconjunto de vizinhos para cada célula e as adicionam como cláusulas no arquivo clauses.wcnf.
    Cláusulas são geradas separadamente para células vivas e mortas em cada geração.
    Para bordas e cantos, que possuem menos vizinhos, funções específicas geram cláusulas considerando os vizinhos válidos.
    São adicionadas clausulas "soft" de peso 1 para cada célula do mapa negada, assim priorizando soluções com menos células vivas.

Resolução:

    MaxSAT Solver: É feita a chamada pelo comando system() do solver open-wbo. O processo é interrompido após encontrar uma solução ótima, UNSAT ou ao atingir o tempo limite definido (5 minutos), onde ele responde com a melhor solução encontrada nesse intervalo.

Organização dos arquivos:

    clauses.h e clauses.c: contém as definições e implementações para gerar as cláusulas do problema.
    test.h e test.c: funções para verificar e simular as transições entre estados do Game of Life.
    utils.h e utils.c: utilitários gerais para manipulação de matrizes, leitura de entradas, escrita de saídas e interação com os arquivos de solução.
    goingBack.c: arquivo principal que coordena a execução do programa, invocando funções de geração de cláusulas e integração com os solvers.
    Makefile: automação para compilar e gerar executável.

Observações:

Além da implementação com o MaxSAT solver também há a implementação usando um SAT solver padrão, na função findBestSolution(). Uma solução alternativa, que busca uma solução qualquer às cláusulas, e ao encontrar, insere ela negada como uma cláusula como entrada novamente para o solver, apenas parando quando a melhor solução for encontrada. Essa solução se mostrou bem mais lenta, principalmente em situações de matrizes maiores que 15x15 no tempo limite de 5 minutos.

Compilação e execução:

    1. Compile o programa:
        make

    2. Execute o programa com um arquivo de entrada:
        ./goingBack input.txt

    * Se executar com a flag "-verify" verifica se a solução está correta: `./goingBack -verify input.txt`

    3. Outputs gerados:
        Impressão no terminal da solução.
        solution/clauses.wcnf: arquivo de cláusulas.
        solution/solution.txt: estado anterior encontrado.

    