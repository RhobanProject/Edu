#!/bin/bash

tmpfile=`tempfile`
license=license.txt
test_presence="creativecommons"

# Process a file
function process
{
    grep -q $test_presence $1

    if [ ! $? -eq 0 ]; then
        cat $license > $tmpfile
        cat $1 >> $tmpfile
        echo "* Adding license to $1..."
        mv $tmpfile $1
    fi
}

# Process all the .cpp and .h
for file in `find . -name "*.cpp" -or -name "*.h" -exec echo {} \;`; do
    process $file
done

rm -rf $tmpfile
