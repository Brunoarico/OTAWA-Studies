#!/bin/bash

for arquivo in *.txt; do
  linha=$(tail -n 1 "$arquivo")
  echo "$linha"
  for ((i=$(wc -l < "$arquivo"); i<200; i++)); do
    echo "$linha" >> "$arquivo"
  done
done
