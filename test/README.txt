Execute intensive (add/mul/conversion) test as:
./test_acc -a
./test_acc -m
./test_acc -c


# Run multiple experiments
./test_acc -c > convert_16_1.txt & ./test_acc -m > multiply_16_1.txt & ./test_acc -a > addition_16_1.txt &
./test_acc -c > convert_16_2.txt & ./test_acc -m > multiply_16_2.txt & ./test_acc -a > addition_16_2.txt &