#!/bin/bash
echo "Gerando arquivos testeX.out ..."
NUMERO=0
if [ ! -d "/saidas" ]; then
    mkdir saidas
fi
while [ -e entradas/teste$NUMERO.in ]
do
    value=`cat entradas/teste$NUMERO.in`
    echo "$value" | ./main > saidas/teste$NUMERO.out
    echo "Arquivo teste"$NUMERO".out gerado com sucesso!"
    NUMERO=$((NUMERO+1))
done
