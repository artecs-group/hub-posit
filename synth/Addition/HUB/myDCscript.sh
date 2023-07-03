#!/bin/bash

echo "My dc_shell script"

# declare -A constraints=( [8]=1.0 [16]=2.0 [32]=3.0 )


for w in 8 16 #32
do
# try different timing constraints
for f in unconstrained 200 250 300 350 400 450 500 550 600 650 700 750 800 850 900 950 1000
do
	if [ $f = 'unconstrained' ]; then
		dir="${f}"
	else
		dir="${f}_MHz"
		# Compute clock period from frequency
		freq=$(awk -v c=$f 'BEGIN {print (1000 / c) }')
	fi

	rm -rf "${w}bits/${dir}/"
	mkdir -p "${w}bits/${dir}/"
	cp s1 "${w}bits/${dir}/"
	cd "${w}bits/${dir}/"
	# Limit delay
	if [ $f != 'unconstrained' ]; then
		sed -i "s/\#set_max_delay 1 \[all_outputs\]/set_max_delay ${freq} [all_outputs]/g" s1
	fi
	echo ""
	echo ""
	echo "******** item: ${w} bits - ${dir}"
	echo ""
	echo ""
	dc_shell-t -f s1
	#ls *.vhdl
	cd ../..
done
done
