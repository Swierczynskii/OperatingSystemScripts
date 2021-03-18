mkdir dir1
touch file.file
cd dir1
mkdir dir2
mkdir dir3
touch test.test
touch test.txt
touch testing
touch file
cd dir2 
touch file2
touch file3
cd ..
cd dir3
touch file4
touch file5
cd ..
cd ..

echo "Using: ./modify.sh -h"
./modify.sh -H
echo "=================================================
Testing uppercasing/lowercasing:

dir1 before using modify.sh:"
ls dir1
echo "
Uppercasing test.test in dir1: ./modify.sh -u dir1/test.test
dir1:"
./modify.sh -u dir1/test.test
ls dir1
echo "
Lowercasing TEST.TEST in dir1: ./modify.sh -l dir1/TEST.TEST
dir1:"
./modify.sh -l dir1/TEST.TEST
ls dir1

rm file.file
rm -r dir1

