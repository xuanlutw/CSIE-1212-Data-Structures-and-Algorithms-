make
cd test
../a.out < p02_input01.txt > out01
diff p02_output01.txt out01
rm out01 ../a.out
