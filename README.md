ShaDaPP
=======

Share Data by Peer to Peer

Required tools
--------------

- A C++11 compiler, like [GCC](http://gcc.gnu.org/) or [Clang](http://clang.llvm.org/)
- [CMake](http://www.cmake.org/) (version >= 2.8.8)
- [Qt](http://qt-project.org/) (version >= 4, modules : QtCore, QtNetwork, QtXml, QtXmlPatterns)
- [LZ4](https://code.google.com/p/lz4/) (version >= r124)

Build & run
-----------

Using CMake manually :

``` bash
# Get and build
git clone https://github.com/jdussouillez/ShaDaPP
cd ShaDaPP
mkdir build
cd build
cmake ../src
make

# Run ShaDaPP
./bin/shadapp/shadapp [ARGS]
```

Using the Bash script :

``` bash
# Get and build
git clone https://github.com/jdussouillez/ShaDaPP
cd ShaDaPP
./auto.sh b

# Run ShaDaPP
./auto.sh r [ARGS]
```
Note: Use the script's option "--help" for more information.
