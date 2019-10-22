#!/bin/bash
key="$1"
value="$2"

fifo_salida="/home/colby/fifo_salida"
fifo_entrada="/home/colby/fifo_entrada"
echo -n "$key=$value" > "$fifo_entrada" #entrada del server
sleep 1

[ -p $fifo_salida ] || mkfifo "$fifo_salida"


while read line <"$fifo_salida";
do
if [[ "$line" == "16" ]] 
then

 echo "Fin de la transmision de datos."
	exit 1
fi

 echo "$line"

done 






