>input.txt 
>output.txt
	
for n in {0..100}; do 
	echo fibonacci $(($RANDOM%1000)) >> input.txt 
done

echo "exit" >> input.txt
	
./par-shell < input.txt # | tee output.txt

#printf "Output lines: %d" $(wc -l output.txt)
