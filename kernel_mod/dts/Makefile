KERNELDIR := /home/lrq/linux/IMX6ULL/temp
DTB_PATH := /home/lrq/linux/tftpboot
DTB_FILE := imx6ull-alientek-emmc.dtb

build: dtb

dtb:
	$(MAKE) -C $(KERNELDIR) dtbs
	cp $(KERNELDIR)/arch/arm/boot/dts/$(DTB_FILE) $(DTB_PATH)
