>test.in 
>test.out
	
for n in {0..100}; do 
	echo fibonacci $(($RANDOM%1000)) >> test.in 
done

echo "exit" >> test.in
	
./par-shell < test.in  | tee test.out

echo "Output lines:" $(wc -l test.out)
