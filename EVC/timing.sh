#!/bin/bash


gcc evc.c -o timing.o -lgmp -lpbc

echo "n,k,size_of_operation,CommitSet,MemberP,MemberV,UpdateCommitment,AggP,AggV,EqualP,EqualV*k" >> timing.csv

for i in 10 20 40 80 160 320 640 1280 2560 5120
do
    echo "Timing the case when k = $i and size_of_operation = 10"
	./timing.o $i 10 >> timing.csv
done

for i in 5 10 20 40 80 160 320 640
do
    echo "Timing the case when k = 640 and size_of_operation = $i"
	./timing.o 640 $i >> timing.csv
done
