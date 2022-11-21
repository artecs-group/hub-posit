#!/bin/bash

echo "My timing report script"

# declare -A constraints=( [8]=0.75 [16]=1.75 [32]=2.75 )

touch results/delay.csv
R="$PWD/results/delay.csv"

for w in 32
do
# try different timing constraints
for f in unconstrained 200 250 300 350 400 450 500 550 600 650 700 750 800 850 900 950 1000
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
	grep -e 'data arrival' *timing.report | sed '1q' | awk 'NF>1{printf $NF}' >> $R
	if [ $f = 'unconstrained' ]; then
			echo ',MET' >> $R
	else
			grep -e 'slack' *timing.report | sed 's/[()]//g' | awk '{print "," $(NF-1)}' >> $R
	fi
	
	cd ../..
done
done
