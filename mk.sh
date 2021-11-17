echo "---------------- Secuential Program"
gcc sec.c -o sec && ./sec color.bmp ; rm sec

echo "---------------- Parallel Program"
gcc par.c -o par -fopenmp -lm && ./par color.bmp ; rm par