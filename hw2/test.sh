make

echo 1.in
time ./a.out ./test/public_data.txt < ./test/1.in > ./test/1.out.test
diff ./test/1.out ./test/1.out.test
echo 2.in
time ./a.out ./test/public_data.txt < ./test/2.in > ./test/2.out.test
diff ./test/2.out ./test/2.out.test
echo 3.in
time ./a.out ./test/public_data.txt < ./test/3.in > ./test/3.out.test
diff ./test/3.out ./test/3.out.test
echo 4.in
time ./a.out ./test/public_data.txt < ./test/4.in > ./test/4.out.test
diff ./test/4.out ./test/4.out.test
