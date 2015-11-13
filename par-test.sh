#!/bin/bash

>test.in
>test.out

for n in {0..100}; do
	echo fibonacci $RANDOM  >> test.in
done

echo "exit" >> test.in

echo "starting par-shell now"

./par-shell < test.in

echo "Output lines:" $(wc -l test.out)
