#!/bin/bash

echo "My power report script"

# declare -A constraints=( [8]=0.75 [16]=1.75 [32]=2.75 )

touch results/power.csv
R="$PWD/results/power.csv"

for w in 8 16 #32
do
# try different timing constraints
for f in unconstrained 200 250 300 350 400 450 500 550 600 650 700 750 800 850 900 950 1000 1050 1100 1150 1200
do
	if [ $f = 'unconstrained' ]; then
		dir="${f}"
		freq=0
	else
		dir="${f}_MHz"
		# Compute clock period from frequency
		freq=${f}
	fi

	cd "${w}bits/${dir}/"
	printf ${w},${freq}, >> $R
	grep -e '^Total.*mW$' *power.report | awk 'NF>1{p = $(NF-1) ; print p}' >> $R
	
	cd ../..
done
done
