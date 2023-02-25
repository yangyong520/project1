all:
	@echo ""
	@echo "Start compiling......"
	@echo ""
	gcc -o sub sub.c -lmosquitto
	gcc -o pub pub.c -lmosquitto
	@echo "end"
sub:
	gcc -o sub sub.c -lmosquitto 

pub:
	gcc -o pub pub.c -lmosquitto 

clean:
	-rm sub pub
