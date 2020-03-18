#!/bin/bash

#pocet cisel bud zadan nebo 10, 
if [ $# -lt 1 ];then 
    numbers=10;
else
    if [ $1 -lt 1 ];then    #less than 1 gets defaulted to 10
    numbers=10;else
    numbers=$1;
    fi;
fi;

#preklad cpp zdrojaku
mpic++ --prefix /usr/local/share/OpenMPI -o oets odd-even.cpp


#vyrobeni souboru s random cisly
dd if=/dev/random bs=1 count=$numbers of=numbers

#spusteni
mpirun --prefix /usr/local/share/OpenMPI -np $numbers oets

#uklid
rm -f oets numbers
