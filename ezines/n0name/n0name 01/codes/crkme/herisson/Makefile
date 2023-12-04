all:
	nasm -f elf func.asm -o func.o
	gcc -O3 -fomit-frame-pointer func.o soluce.emp.c -o brutepowa
test:
	gcc -O3 -fomit-frame-pointer soluce2.emp.c -o brutepowa2

clean:
	rm *.o brutepowa brutepowa2
