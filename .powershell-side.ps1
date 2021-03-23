echo "TUTagSat2021.c" | Out-File TUTagSat2021.side -Encoding ASCII
ls *.[ch] | Where-Object {$_.Name -ne "TUTagSat2021.c"} | ForEach-Object {$_.Name} | Out-File -FilePath TUTagSat2021.side -Encoding ASCII -Append
cat .side-compile.txt | Out-File -FilePath TUTagSat2021.side -Encoding ASCII -Append