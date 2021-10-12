# Patch PREEMPT_RT for Linux with Raspberry Pi 4

1. Download the RPi Linux kernel and PREEMPT_RT patch file:

```
mkdir workspace
cd workspace
git clone -b rpi-5.4.y https://github.com/raspberrypi/linux.git
wget https://mirrors.edge.kernel.org/pub/linux/kernel/projects/rt/5.4/older/patch-5.4.54-rt32.patch.gz
```

2. Patch the RPi Linux kernel:
```
cd linux
gzip -cd ../patch-5.4.54-rt32.patch.gz | patch -p1 --verbose
```

3. Initialize the kernel config:
```
make ARCH=arm64 CROSS_COMPILE=aarch64-linux-gnu- bcm2711_defconfig
```

4. Start the menuconfig:
```
make ARCH=arm64 CROSS_COMPILE=aarch64-linux-gnu- menuconfig
```

5. Select **General setup -> Preemption Model -> Fully Preemptible Kernel (Real-Time)**. Remember to save before exit.


6. Start compilation:

```
make -j4 ARCH=arm64 CROSS_COMPILE=aarch64-linux-gnu-
```

7. Export the compiled result:

```
mkdir -p ../rt-linux/boot
export INSTALL_MOD_PATH=../rt-linux/
export INSTALL_DTBS_PATH=../rt-linux/
make ARCH=arm64 CROSS_COMPILE=aarch64-linux-gnu- modules_install dtbs_install
cp arch/arm64/boot/Image ../rt-linux/boot/kernel8.img
```

8. Download the official Raspbian image:

```
cd workspace
wget https://downloads.raspberrypi.org/raspbian/images/raspbian-2020-02-14/2020-02-13-raspbian-buster.zip
```

9. Flash the Raspbian image to the SD card:

```
sudo dd if=./2020-02-13-raspbian-buster.img  of=/dev/sdX bs=4M
```

10. Mount the SD card on the host computer:

```
sudo mkdir -p /media/boot
sudo mkdir -p /media/rootfs
sudo mount /dev/sdX1 /media/boot
sudo mount /dev/sdX2 /media/rootfs
```

11. Copy the compiled results to the SD card:

```
sudo cp rt-linux/boot/kernel8.img /media/boot/
sudo cp -r rt-linux/boot/overlays/ /media/boot/
sudo cp -r lib/modules/5.4.83-rt32-v8+/ /media/rootfs/lib/modules/
```

12. Assign kernel image in **config.txt**:

```
sudo echo "kernel=kernel8.img" >> /media/boot/config.txt
```

13. Unmount the SD card:

```
sudo umount /media/boot
sudo umount /media/rootfs
sudo rm -rf /media/boot/
sudo rm -rf /media/rootfs/
```

14. Finished.