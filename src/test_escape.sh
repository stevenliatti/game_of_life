make
for i in `seq 1 100`; do
	echo $i
	./gameoflife 240 135 0 0.75 30 8
done
make clean
