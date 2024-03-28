gcc:
	arm-linux-gcc ./src/*.c -o main -I./include/ -L./lib/ -ljpeg -lpng -lz -lfont -lm
clear:
	rm main -f 
	rm *.o -f

clear_user_data:
	sudo rm ./user_data/*

create_so:
	arm-linux-gcc -c -fPIC ./src/double_list.c \
	./src/file_opt.c ./src/get_time.c ./src/jpeg.c \
	./src/key_pad.c ./src/lcd.c ./src/master.c  \
	./src/ts.c -I./include/ -L./lib/ -ljpeg -lpng -lz -lfont -lm
	arm-linux-gcc -shared -fPIC -o libmain.so *.o
	