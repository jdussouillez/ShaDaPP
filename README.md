ShaDaPP
=======

Required libraries
------------------

- Qt4, modules : QtCore, QtNetwork, QtXml, QtXmlPatterns (http://qt-project.org/)

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
./bin/client/gzzzt
```

Using the Bash script :

``` bash
# Get and build
git clone https://github.com/jdussouillez/ShaDaPP
cd ShaDaPP
./auto.sh b

# Run ShaDaPP
./auto.sh r
```
Note: Use the script "auto.sh" option "help" for more information
