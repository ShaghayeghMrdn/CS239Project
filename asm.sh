#!/bin/bash
JUNIT=$1
ASM_JAR=$2

for f in $(find $1/target/classes/ -name '*.class');
do
	echo "processing $f..."
	java -cp $2:. ASM_Example $f $f.new
	mv $f $f.old
	mv $f.new $f
done

