mkdir dir1
cd dir1
touch test.test
mkdir dir2
touch blablabla

cd dir2
touch pathtest
cd ..

###Creating tree directory for recursive tests
mkdir rec_dir2
cd rec_dir2 
touch file2
touch file3
mkdir dir10
cd dir10
touch file10
touch file11
cd ..
mkdir dir11
cd dir11
mkdir dir12
touch file12
mkdir dir13
cd dir13
touch file13
###

cd ..
cd ..
cd ..
cd ..

echo "Using: ./modify.sh -h"
./modify.sh -H
echo "=================================================
Testing simple, proper usage:"

echo "
Uppercasing test.test in dir1: ./modify.sh -u dir1/test.test
dir1:"
echo "Before: " 
ls dir1
./modify.sh -u dir1/test.test
echo "After: " 
ls dir1
echo "
Lowercasing TEST.TEST in dir1: ./modify.sh -l dir1/TEST.TEST
dir1:"
echo "Before: " 
ls dir1
./modify.sh -l dir1/TEST.TEST
echo "After: " 
ls dir1
echo "
Sed pattern test in dir2, replace t with g: ./modify.sh 's/test/success/g' dir1/dir2/file2
dir1:"
echo "Before: " 
ls dir1/dir2
./modify.sh 's/test/success/g' dir1/dir2/pathtest
echo "After: "
ls dir1/dir2

echo "=================================================
Testing recursive, proper usage:"
echo "
Before: "
echo "
rec_dir2: "
ls dir1/rec_dir2
echo "
dir10: "
ls dir1/rec_dir2/dir10
echo "
dir11: "
ls dir1/rec_dir2/dir11
echo "
empty dir12: "
ls dir1/rec_dir2/dir11/dir12
echo "
dir13: "
 ls dir1/rec_dir2/dir11/dir13
./modify.sh -r -u dir1/rec_dir2
echo "-----
After: "
echo "
rec_dir2: "
ls dir1/rec_dir2
echo "
dir10: "
ls dir1/rec_dir2/dir10
echo "
dir11: "
ls dir1/rec_dir2/dir11
echo "
empty dir12: "
ls dir1/rec_dir2/dir11/dir12
echo "
dir13: "
ls dir1/rec_dir2/dir11/dir13


rm -r dir1



