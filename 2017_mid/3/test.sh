make
cd test
../a.out < p03_input01.txt > out01
diff p03_output01.txt out01
rm out01 ../a.out
