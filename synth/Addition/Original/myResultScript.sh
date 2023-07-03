#!/bin/bash

# rm -rf results
# mkdir -p results

bash myAreaScript.sh
bash myPowerScript.sh
bash myTimeScript.sh

python ../../elaborate_json.py $PWD/results $PWD

