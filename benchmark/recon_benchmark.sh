
scheme=$1

echo "Running for Scheme $scheme"
# params m
max_m=1001

for n in 8
do
	for t in 2 3 5 7 $(seq 10  10 $(expr $max_m - 1));
	do
		if [[ $scheme -eq 1 ]]
		then
			stepsize=$((50 / (2 ** ($t - 2))))
		elif [[ $scheme -eq 2 ]]
		then
			stepsize=$((5 / (2 ** ($t - 2))))
		fi
		stepsize=$(( stepsize > 0 ? stepsize : 1 ))
		echo "Stepsize set to $stepsize"

		for m in $(seq $(expr $t + 1) $stepsize $max_m);
		do
			echo "m $m n $n t $t :"
			./benchmark all -m $m -n $n -t $t -x -y -s $1
			timeout 3h ./benchmark recon -m $m -n $n -t $t -l -s $1 -r 3
			if [ $? -eq 124 ]; then 
  				echo "the command timed out for m $m n $n t $t"
				break
			fi
		done
	done
done
