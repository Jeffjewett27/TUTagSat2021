#! /bin/bash
echo "TUTagSat2021.c" > TUTagSat2021.side
ls *.[ch] | grep -e "TUTagSat2021.c" -v >> TUTagSat2021.side
cat .side-compile.txt >> TUTagSat2021.side
