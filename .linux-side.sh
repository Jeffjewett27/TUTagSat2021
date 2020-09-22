#! /bin/bash
echo "TUSatellite7.c" > TUSatellite7.side
ls *.[ch] | grep -e "TUSatellite7.c" -v >> TUSatellite7.side
cat .side-compile.txt >> TUSatellite7.side
