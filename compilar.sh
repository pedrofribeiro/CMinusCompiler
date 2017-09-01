clear all

echo =============================================================================================================
echo Inicio
echo =============================================================================================================

bison -d -v tiny.y

echo -------------------------------------------------------------------------------------------------------------

flex tiny.l

echo -------------------------------------------------------------------------------------------------------------

#flags padrao
#gcc -o tiny *.c -ly -lfl  

ulimit -c unlimited
gcc -g -o tiny *.c -ly -lfl -Wall
#gdb generate-core-file ./tiny
#-g é a flag que gera o arquivo de execução (trace)
#"core", que deve ser analisado usando o gdb
#printf com referencia explicita vazia gera segmentation fault não-identificavel
#algo da forma: printf("%s",);

echo -------------------------------------------------------------------------------------------------------------

#./tiny selectionSort.txt
#./tiny gcd.cm
#./tiny fib.cm
#./tiny sort.cm
#./tiny foo.cm
./tiny soma.cm

echo =============================================================================================================
echo Fim
echo =============================================================================================================
