obj-m:=gpio_driver.o
KDIR:=/home/simba/Documents/project/3326/kernel #内核的具体目录
PWD:=$(shell pwd)

all:
	make ARCH=arm64 CROSS_COMPILE=./../prebuilts/gcc/linux-x86/aarch64/gcc-linaro-6.3.1-2017.05-x86_64_aarch64-linux-gnu/bin/aarch64-linux-gnu- -C $(KDIR) M=$(PWD) modules
	adb push gpio_driver.ko /userdata/simba

clean:
	rm -rf .*.cmd *.o *.mod.c *.ko .tmp_versions *.order *symvers *Module.markers