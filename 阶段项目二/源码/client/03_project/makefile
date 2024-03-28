CC = arm-linux-gcc

TARGET = main

DEPENDS = ./main/main.c \
		./jpeg/read_jpeg.c \
		./lcd/lcd.c \
		./data_structure/data_struct.c \
		./touch/touch.c \
		./player/video_player.c \
		./file_opt/dir_scan.c \
		./network/network.c \
		./thread/my_thread.c \
		./gui/gui.c \
		./font/font.c \
		./png/src/read_png.c \
		./other/other.c \

CFLAGS = -I./jpeg/include/ \
		-I./lcd/ \
		-I./touch/ \
		-I./player/ \
		-I./image/ \
		-I./file_opt/ \
		-I./network/ \
		-I./thread/ \
		-I./font/ \
		-I./gui/ \
		-I./other/ \
		-I./main/ \
		-I./data_structure/ \
		-I./png/include/ \
		-L./jpeg/lib -ljpeg \
		-L./font/ -lfont -lm \
		-L./png/lib/ -lpng -lz \
		-lpthread

arm:
	$(CC) $(DEPENDS) $(CFLAGS) -o ./output/$(TARGET)

clean:
	rm -rf $(TARGET)
