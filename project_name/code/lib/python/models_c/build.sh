#!/bin/bash
#this will remove any partial builds, if there was a failure


if [ "$1" == "clean" ]
then
    rm ext_func/*.so
    exit
fi


python2 setup.py build_ext --inplace
#python3 setup.py build_ext --inplace
mv -f *.so ext_func
rm -r build/
echo "~~You made it!~~"
