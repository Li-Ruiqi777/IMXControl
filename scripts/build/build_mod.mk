KERNELDIR := /home/lrq/linux/IMX6ULL/temp
CURRENT_PATH := $(shell pwd)
APPLICATION_EMBED_MODUILES := /home/lrq/linux/nfs/qtrootfs/home/modules

build: kernel_modules
	mkdir -m 755 -p ${APPLICATION_EMBED_MODUILES}/
	mv *.ko ${APPLICATION_EMBED_MODUILES}/
ifeq ($(FIRMWARE_CLEAN),1)
	make clean
endif

kernel_modules:
	$(MAKE) -C $(KERNELDIR) M=$(CURRENT_PATH) modules

clean:
	$(MAKE) -C $(KERNELDIR) M=$(CURRENT_PATH) clean
