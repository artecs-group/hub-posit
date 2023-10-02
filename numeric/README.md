This sources rely on the [Universal Numbers Library](https://github.com/stillwater-sc/universal/tree/main). Modify the Makefile to include the directory whith the library headers.

Build the program with
```bash
mkdir build && cd build
cmake .. && make
```

Execute intensive (add/mul/conversion) test as:
```bash
./test_acc -a > addition_16.txt
./test_acc -m > multiply_16.txt
./test_acc -c > convert_16.txt
```


<!-- 
# Run multiple experiments
./test_acc -c > convert_16_1.txt & ./test_acc -m > multiply_16_1.txt & ./test_acc -a > addition_16_1.txt &
./test_acc -c > convert_16_2.txt & ./test_acc -m > multiply_16_2.txt & ./test_acc -a > addition_16_2.txt & 
-->