make
cd test
../a.out < p04_input01.txt > out01
../a.out < p04_input02.txt > out02
../a.out < p04_input03.txt > out03
../a.out < p04_input04.txt > out04
diff p04_output01.txt out01
diff p04_output02.txt out02
diff p04_output03.txt out03
diff p04_output04.txt out04
rm out01 out02 out03 out04 ../a.out
