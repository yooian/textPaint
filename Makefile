paint.out: main.o paint.o canvas.o userInput.o
	gcc -g -Wall -Werror -Wextra -o paint.out main.o paint.o canvas.o userInput.o

main.o: main.c userInput.h canvas.h paint.h
	gcc -g -Wall -Werror -Wextra -c main.c

paint.o: paint.c paint.h canvas.h
	gcc -g -Wall -Werror -Wextra -c paint.c

canvas.o: canvas.c canvas.h
	gcc -g -Wall -Werror -Wextra -c canvas.c

userInput.o: userInput.c userInput.h
	gcc -g -Wall -Werror -Wextra -c userInput.c

clean:
	rm -fr *.o *.out
