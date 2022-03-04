
BASEINCLUDE ?= /lib/modules/`uname -r`/build


obj-m	:= query_process_param.o

all : 
	$(MAKE) -C $(BASEINCLUDE) M=$(PWD) modules;

clean:
	$(MAKE) -C $(BASEINCLUDE) M=$(PWD) clean;
	rm -f *.ko;
