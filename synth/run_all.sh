for k in Addition Multiplication
do
  cd $k
  for d in Original HUB
  do
    cd $d
    echo $PWD
    bash myDCscript.sh && \
    bash myResultScript.sh
    cd ..
  done
  python gen_synopsys_json.py
  cd ..
done
