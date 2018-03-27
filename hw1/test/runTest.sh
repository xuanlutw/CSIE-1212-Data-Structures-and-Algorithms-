#!/bin/bash

cd ..
make

./a.out construct > test/output/construct.output
./a.out index >  test/output/index.output
./a.out assign >  test/output/assign.output
./a.out unary >  test/output/unary.output
./a.out add >  test/output/add.output
./a.out subtract >  test/output/subtract.output
./a.out mult >  test/output/mult.output
./a.out division >  test/output/division.output
./a.out io >  test/output/io.output

echo add
diff test/outputStd/add.output test/output/add.output
echo assign
diff test/outputStd/assign.output test/output/assign.output
echo construct
diff test/outputStd/construct.output test/output/construct.output
echo division
diff test/outputStd/division.output test/output/division.output
echo index
diff test/outputStd/index.output test/output/index.output
echo io
diff test/outputStd/io.output test/output/io.output
echo substract
diff test/outputStd/subtract.output test/output/subtract.output
echo unary
diff test/outputStd/unary.output test/output/unary.output
