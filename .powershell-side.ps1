echo "TUSatellite7.c" | Out-File TUSatellite7.side -Encoding ASCII
ls *.[ch] | Where-Object {$_.Name -ne "TUSatellite7.c"} | ForEach-Object {$_.Name} | Out-File -FilePath TUSatellite7.side -Encoding ASCII -Append
cat .side-compile.txt | Out-File -FilePath TUSatellite7.side -Encoding ASCII -Append