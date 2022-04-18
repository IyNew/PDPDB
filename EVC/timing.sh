#!/bin/bash


gcc evc.c -o timing.o -lgmp -lpbc

echo "n,k,size_T,CommitSet (s),MemberProve (s),MemberVerify (s),UpdateCommit (s),AggProof (s),AggVerify (s),EqualProve (s),EqualVerify (s)" >> timing.csv

for i in 40 80 160 320 640 1280 2560 5120 10240 20480 40960 81920
do
	echo "Timing the case when k = $i and size_T = 40"
	./timing.o $i 40 >> timing.csv
done

for i in 40 80 160 320 640 1280 2560 5120
do
    echo "Timing the case when k = 81920 and size_T = $i"
	./timing.o 81920 $i >> timing.csv
done
