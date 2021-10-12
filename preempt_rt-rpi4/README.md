# Patch Linux with PREEMPT_RT for Raspberry Pi 4

0. Install prerequisite software

```
sudo apt-get install gcc-9-aarch64-linux-gnu
```

1. Download the RPi Linux kernel and PREEMPT_RT patch file:

```
mkdir -p ~/rpi-ws
cd ~/rpi-ws
git clone -b rpi-5.4.y https://github.com/raspberrypi/linux.git
wget https://mirrors.edge.kernel.org/pub/linux/kernel/projects/rt/5.4/older/patch-5.4.54-rt32.patch.gz
```

2. Patch the RPi Linux kernel:
```
cd ~/rpi-ws/linux
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
mkdir -p ~/rpi-ws/out/boot
export INSTALL_MOD_PATH=~/rpi-ws/out/
export INSTALL_DTBS_PATH=~/rpi-ws/out/
make ARCH=arm64 CROSS_COMPILE=aarch64-linux-gnu- modules_install dtbs_install
cp arch/arm64/boot/Image ~/rpi-ws/out/boot/kernel8.img
```

8. Download the official Raspbian image:

```
cd ~/rpi-ws/
wget https://downloads.raspberrypi.org/raspbian/images/raspbian-2020-02-14/2020-02-13-raspbian-buster.zip
```

9. Flash the Raspbian image to the SD card:

```
sudo dd if=./2020-02-13-raspbian-buster.img  of=/dev/sdX bs=4M
```

10. Mount the SD card on the host computer:

```
mkdir -p ~/rpi-ws/sd/boot
mkdir -p ~/rpi-ws/sd/rootfs
sudo mount /dev/sdX1 ~/rpi-ws/sd/boot
sudo mount /dev/sdX2 ~/rpi-ws/sd/rootfs
```

11. Copy the compiled results to the SD card:

```
cp ~/rpi-ws/out/boot/kernel8.img ~/rpi-ws/sd/boot/
cp -r ~/rpi-ws/out/overlays/ ~/rpi-ws/sd/boot/
cp -r ~/rpi-ws/out/broadcom/ ~/rpi-ws/sd/boot/
cp -r ~/rpi-ws/out/lib/modules/5.4.83-rt32-v8+/ ~/rpi-ws/sd/rootfs/lib/modules/
```

12. Assign kernel image in **config.txt**:

```
sudo echo "kernel=kernel8.img" >> ~/rpi-ws/sd/boot/config.txt
```

13. Unmount the SD card:

```
sudo umount ~/rpi-ws/sd/boot
sudo umount ~/rpi-ws/sd/rootfs
```

14. Finished.
