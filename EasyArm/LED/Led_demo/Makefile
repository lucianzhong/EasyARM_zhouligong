
CFLAGS += -Wall
obj := led 
src := led.c
CC  := arm-fsl-linux-gnueabi-gcc 

$(obj): $(src)
	$(CC) $(CFLAGS) $^ -o $@ -g

.PHONY: clean
clean:
	-rm $(obj) 
