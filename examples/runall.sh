#!/bin/bash

list=`ls -d */ | sed  's/\///g'`

for next in $list
do
  echo $next
  cd $next
  qmake
  make distclean
  qmake
  make -j15
  ./${next}
  cd ..
done
