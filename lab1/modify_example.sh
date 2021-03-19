#Script used to execution of modify.sh with simple tests

###Creating few files for simple tests
mkdir dir1
cd dir1
touch test.c
mkdir dir2
cd dir2
touch pathtest
cd ..

###Creating tree of files for recursive tests
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
echo "=================================================
TESTING: ./modify.sh -h"
./modify.sh -H
echo "=================================================
TESTING: SIMPLE, PROPER USAGE:"

echo "
Uppercasing test.c in dir1: ./modify.sh -u dir1/test.c
dir1:"
echo "Before: " 
ls dir1
./modify.sh -u dir1/test.c          # Uppercasing test.c file
echo "After: " 
ls dir1
echo "
Lowercasing TEST.C in dir1: ./modify.sh -l dir1/TEST.C
dir1:"
echo "Before: " 
ls dir1
./modify.sh -l dir1/TEST.C          # Lowercasing TEST.Cfile
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
TESTING: RECURSIVE, PROPER USAGE:"
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
./modify.sh -r 's/f/Frag/g' dir1/rec_dir2
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

echo "=================================================
TESTING: SIMPLE, IMPROPER USAGE:"
echo "
Uppercasing non-existing file in dir1: ./modify.sh -u dir1/blabla.c
dir1:"
echo "Before: " 
ls dir1
./modify.sh -u dir1/blabla.c          # Uppercasing test.c file
echo "After: " 
ls dir1
echo "
Uppercasing existing file in non-existing dir0: ./modify.sh -u dir0/test.c
dir1:"
echo "Before: " 
ls dir1
./modify.sh -u dir0/test.c          
echo "After: " 
ls dir1
echo "
Wrong input into dir1: ./modify.sh 'blablabla' dir1/test.c    
dir1:"
echo "Before: " 
ls dir1
./modify.sh blablabla dir1/test.c         
echo "After: " 
ls dir1
echo "
Wrong input into dir1: ./modify.sh 's/bla/failure/g' dir1/test.c    
dir1:"
echo "Before: " 
ls dir1
./modify.sh 's/bla/failure/g' dir1/test.c         
echo "After: " 
ls dir1
echo "
No files given: ./modify.sh -l dir1
dir1:"
echo "Before: " 
ls dir1
./modify.sh -l dir1         
ls dir1
echo "
No directory/files given: ./modify.sh -l"
./modify.sh -l              


rm -r dir1
