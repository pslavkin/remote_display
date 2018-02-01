port=/dev/ttyUSB1
A="./blhost -p $port,57600 -t 1 flash-erase-all-unsecure"
B="./blhost -V -p $port,57600 --timeout 100 get-property 1"
C="./blhost  -d -p $port,57600 -t 500 write-memory 0 ../exe/app.bin"
D="./blhost  -p $port,57600 -t 500 reset"


$A
while [ $? == 1 ]; do
	$A;
done
$C
$D

	
