#!/bin/bash

_grep='../../cmake-build-release/weak_grep'

dont_clean=(no thing)

test_file="all.txt"

python_def_pattern=' *def [a-z_][a-z0-9_]*\( *( *[a-z_][a-z0-9_]*( *: *[a-z_][a-z0-9_]*)? *,?)* *( *[a-z_][a-z0-9_]*( *: *[a-z_][a-z0-9_]*)? *= *[a-z_][a-z0-9_]*,?)*\):'

patterns=("$python_def_pattern"  'exce[a-z]{2}' 'iostream' 'i[os]{2}tream' 'i(ostrea)m' '(iostrea)*m' '(iostrea)+m?'\
          '(ios(t(r)ea))*m' '^#include' 'i([os]{1,3})*tream' ' *#include <iostream> +([^ ]+) +([^ ]+) +([^ ]+)')

END=${#patterns[@]}

for i in $(seq 0 "$END");
do
  echo;
  echo pattern: \'"${patterns[$i]}"\';

  $_grep "${patterns[$i]}" < "$test_file" > test"$i".txt;
  grep -E "${patterns[$i]}" < "$test_file" > test"$i".check.txt;

  compare=$(diff test"$i".txt test"$i".check.txt | wc -l);

  if test "$compare" -eq 0;
  then
    num=$(wc -l < test"$i".txt);
    echo Test "$i" passed \("$num" lines\);
  else
    num_real=$(wc -l < test"$i".txt);
    num_expected=$(wc -l < test"$i".check.txt);
    echo Test "$i" failed \("$num_real" vs "$num_expected" lines\)
    exit "$i";
  fi;

  [[ " ${dont_clean[*]} " =~ $i ]] || rm test"$i".txt test"$i".check.txt;

done