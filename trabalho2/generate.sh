#!/bin/bash
echo "Gerando os filtros ..."
if [ ! -d "out" ]; then
    mkdir out
fi
if [ ! -d "doc" ]; then
    mkdir doc
fi
IMGS=0
for entry in "data/"/*
do
    IFS='/' read -ra ADDR <<< "$entry"
    NUMERO=1
    echo "linear|${ADDR[2]}" | tr '\n' '|'  >> doc/linear.txt
    echo "thread|${ADDR[2]}" | tr '\n' '|'  >> doc/thread.txt
    echo "process|${ADDR[2]}" | tr '\n' '|'  >> doc/process.txt
    while (( $NUMERO < 100 ))
    do
        echo "Imagem ${ADDR[2]} - Iteracao $NUMERO/100"
        echo "Executando o linear..."
        echo "${ADDR[2]}" | ./linear  >> doc/linear.txt
        echo "Executando o multi_thread..."
        echo "${ADDR[2]}" | ./multi_thread  >> doc/thread.txt
        echo "Executando o multi_process..."
        echo "${ADDR[2]}" | ./multi_process  >> doc/process.txt
        NUMERO=$(( NUMERO+1 ))
    done
    echo "" >> doc/linear.txt
    echo "" >> doc/thread.txt
    echo "" >> doc/process.txt
    IMGS=$(( IMGS+1 ))
done

echo "${IMGS}" | python3 src/plot.py