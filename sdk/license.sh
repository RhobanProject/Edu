#!/bin/bash

tmpfile=`tempfile`
license=`echo $0|sed -s 's/\.sh/\.txt/g'`
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
for file in `find . \( -name "*.cpp" -o -name "*.h" \) -exec echo {} \;`; do
    echo "Processing $file"
    process $file
done

rm -rf $tmpfile
