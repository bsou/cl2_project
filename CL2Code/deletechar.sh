#!/bin/sh
for i in *.txt; do tr '/' ' ' < $i > $i.new; done
rm *.txt
for x in *.new; do mv $x $(echo ${x%*.*});done
for i in *.txt; do tr '.' ' ' < $i > $i.new; done
rm *.txt
for x in *.new; do mv $x $(echo ${x%*.*});done
