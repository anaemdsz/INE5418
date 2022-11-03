# Lab 02 - RMI

Trabalho realizado para a disciplina INE5418 - Computação Distribuída, no semestre 22.2.

Alunos: 
* Paola de Oliveira Abel
* Pedro Henrique Aquino Silva 

## Instruções para uso
Esta atividade foi realizada utilizando a versão 11.0.15 do OpenJDK Runtime Enviroment, uma vez que versões mais atuais apresentavam um erro no registro do objeto no servidor. Desta forma, também está incluído junto ao código fonte do projeto um arquivo YAML do ambiente `conda` empregado para desenvolvimento e execução do projeto.

Para facilitar a execução, também são incluídos scripts Bash para execução do servidor (`server.sh`) e de cada cliente (`client.sh`, `client1.sh` e `client2.sh`).

O arquivo `client.sh` (`ClientList.java`) contém um caso simples utilizado para teste; enquanto os clientes `client1.sh` e `client2.sh` devem ser executados um após o outro, demonstrando o caráter compartilhado da lista distribuída.

Antes de executar, também é necessário compilar os arquivos `.java`. Também incluímos um script (`compile.sh`) para facilitar, mas pode-se simplesmente rodar `javac *.java` no diretório.

## Considerações

1) A implementação da lista foi feita utilizando como base a estrutura de `SynchronizedList` do Java, a partir de um `ArrayList`.

As operações da lista são implementadas sobre o tipo `Object`, uma vez que usar genéricos se mostrou um pouco complicado nos registros de objeto remoto.

Para utilizar de *stub*, também foi criada a interface `ListInterface`, enquanto os métodos sobre a lista são implementados na `RemoteList`.

2) O `ServerList` faz o registro de objetos do tipo lista implementado, sendo que, na arquitetura atual, todos os clientes têm acesso à uma única lista, que armezena dados do tipo `Object`. 
   
Isto foi feito de modo que os clientes pudessem armazenar tipos arbitrários de dados. Cada cliente implementa uma classe `ClientList` (são 3, no total, `ClientList`, `Writer` e `Tearer`).

3) A solução empregada é baseada no método `Collections.synchronizedList`, que garante que a lista será *thread-safe*. Deste modo, o acesso concorrente implica em um `lock` da lista quando algum processo cliente faz uma requisição ao servidor. Os clientes então, ficariam travados à espera de uma resposta do servidor. 

Caso um processo faça uma requisição ao servidor e falhe, caberá, então, à implementação interna dos blocos `synchronized` da JVM garantir que os demais processos não ficarão travados eternamente.

