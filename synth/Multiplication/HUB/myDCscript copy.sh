#!/bin/bash

echo "My dc_shell script"

declare -A constraints=( [8]=0.75 [16]=1.75 [32]=2.75 )


for w in 8 16 32
do
# try 3 different timing constraints, reducing 0.25ns each iteration
for f in unconstrained clk clk clk
do
	if [ $f = 'clk' ]; then
		dir="${f}_${constraints[${w}]}_ns"
	else
		dir="${f}"
	fi

	rm -rf "${w}bits/${dir}/"
	mkdir -p "${w}bits/${dir}/"
	cp s1 "${w}bits/${dir}/"
	cd "${w}bits/${dir}/"
	# Limit frequency
	if [ $f = 'clk' ]; then
		if [ $w = '8' ]; then
			sed -i "s/\#set_max_delay 1 \[all_outputs\]/set_max_delay ${constraints[${w}]} [all_outputs]/g" s1
		elif [ $w = '16' ]; then
			sed -i "s/\#set_max_delay 1 \[all_outputs\]/set_max_delay ${constraints[${w}]} [all_outputs]/g" s1
		else
			sed -i "s/\#set_max_delay 1 \[all_outputs\]/set_max_delay ${constraints[${w}]} [all_outputs]/g" s1
		fi
	fi
	echo ""
	echo ""
	echo "******** item: ${w} bits - ${dir}"
	echo ""
	echo ""
	dc_shell-t -f s1
	#ls *.vhdl
	cd ../..

	# Update constraint
	if [ $f = 'clk' ]; then
		constraints[${w}]=$(awk -v a=${constraints[${w}]} 'BEGIN { b = 0.25; print (a - b) }')
	fi
done
done
