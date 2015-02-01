#g++ extract_by_scan.cpp -o test
g++ extract_by_suffix.cpp -o test
#./test 1.txt 6 5 0.9 200 > result.txt&
./test 1.txt 6 1 0 0 > result.txt&
sort -t$'\t' -k 4 -k 3 -nr result.txt -o result.txt
