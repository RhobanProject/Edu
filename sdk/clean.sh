#!/bin/bash

tmpfile=`tempfile`
# Process a file
function process
{
    cat $1|sed -s 's/\r//g' > $tmpfile
    cat $tmpfile > $1
}

# Process all the .cpp and .h
for file in `find . \( -name "*.cpp" -o -name "*.h" \) -exec echo {} \;`; do
    echo "Processing $file"
    process $file
done

rm -rf $tmpfile
