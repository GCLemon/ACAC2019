dir=`dirname $[0]`
g++ -std=c++17 -O -o bin/Shooting `find $dir/src -name *.cpp` lib/libAltseed.a -llua

cd bin
./Shooting
cd ..