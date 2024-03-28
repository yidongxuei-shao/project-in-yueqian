TARGET = main

CFLAGES = ./main/main.c \
		  ./data_struct/double_list.c \
		  ./data_struct/link_list.c \
		  ./file_opt/file_opt.c \
		  ./network/network.c \
		  ./network/cJSON/cJSON.c \
		  ./other/other.c
INCLUDE = -I./main/ \
		  -I./data_struct/ \
		  -I./file_opt/ \
		  -I./network/cJSON/ \
		  -I./network/ \
		  -I./other/
gcc:
	$(CC) $(CFLAGES) $(INCLUDE) -o ./output/$(TARGET)