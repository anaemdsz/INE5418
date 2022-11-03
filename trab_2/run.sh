#!/bin/bash
javac *.java &&

###
### CLIENT CLI
###

while :; do

    echo "Qual função deve rodar agora?"
    echo "0 - Sair"
    echo "1 - Escritor"
    echo "2 - Leitor"
    echo "3 - Apagador"
    echo "4 - Random OP"

    read option
    echo ""

    if [[ $option -eq 4 ]]; then
        option=$((1 + $RANDOM % 3))
    fi

    case $option in
        0) break ;;
        1) java -Djava.security.policy=server.policy Writer ;;
        2) java -Djava.security.policy=server.policy Reader  ;;
        3) java -Djava.security.policy=server.policy Tearer ;;
    esac
    echo ""
    echo ""
done