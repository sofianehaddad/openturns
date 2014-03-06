#!/bin/sh
# update copyright date
newyear=`date +%Y`
oldyear=`expr $newyear - 1`
for subdir in lib python cmake wrappers
do
  grep -lr "2005-$oldyear Airbus-EDF-Phimeca" $subdir|grep -v \.svn|grep -v '~'|xargs sed -i "s|2005-$oldyear Airbus-EDF-Phimeca|2005-$newyear Airbus-EDF-Phimeca|g"
done
