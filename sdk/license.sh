#!/bin/bash

tmpfile=`tempfile`
license=`echo $0|sed -s 's/\.sh/\.txt/g'`
test_presence="creativecommons"
test_other="sourceforge"

# Process a file
function process
{
    grep -q $test_presence $1
    if [ $? -eq 0 ]; then
        return
    fi

    grep -q $test_other $1
    if [ $? -eq 0 ]; then
        return
    fi
    
    echo $1|grep -q 'yaml-cpp'
    if [ $? -eq 0 ]; then
        return
    fi

    cat $license > $tmpfile
    cat $1 >> $tmpfile
    echo "* Adding license to $1..."
    mv $tmpfile $1
}

# Process all the .cpp and .h
for file in `find . \( -name "*.cpp" -o -name "*.h" \) -exec echo {} \;`; do
    echo "Processing $file"
    process $file
done

rm -rf $tmpfile
