> input.txt

# echo "inputs:"; read n_inputs
n_inputs=100

for n in $(seq 1 $n_inputs); do
	echo fibonacci $RANDOM >> input.txt
done
echo "exit" >> input.txt
