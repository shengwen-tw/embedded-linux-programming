# Emulate Raspberry Pi 3 with QEMU

## 1. Build and install QEMU 6.1

```
sudo apt-get install gcc build-essential automake \
gcc-arm-linux-gnueabihf vim git wget python \
pkg-config zlib1g-dev libglib2.0-dev libpixman-1-dev \
flex bison unzip libncurses5-dev ninja-build

wget https://download.qemu.org/qemu-6.1.0.tar.xz
tar xvf qemu-6.1.0.tar.xz
make -j$(nproc)
sudo make install
```

## 2. Download the Raspbian image

```
#For convenient, we save our virtual machine files in this directory:
cd embedded-linux-programming/qemu-rpi3/

wget https://downloads.raspberrypi.org/raspbian/images/raspbian-2018-11-15/2018-11-13-raspbian-stretch.zip
unzip 2018-11-13-raspbian-stretch.zip
```

## 3. Patch and compile the Linux kernel for QEMU

1. Download the Linux kernel soruce code:

```
#For convenient, we save our virtual machine files in this directory:
cd embedded-linux-programming/qemu-rpi3/

git clone git://github.com/raspberrypi/linux.git --branch raspberrypi-kernel_1.20180619-1 --single-branch --depth 1
```

2. Patch the kernel:

```
cd linux
wget https://raw.githubusercontent.com/dhruvvyas90/qemu-rpi-kernel/master/tools/linux-arm.patch
patch -p1 < ./linux-arm.patch
```

3. Setup kernel configuration:

```
#Initialize the menuconfig
KERNEL=kernel7
make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- versatile_defconfig

#Download the config files for QEMU
wget https://raw.githubusercontent.com/dhruvvyas90/qemu-rpi-kernel/master/tools/config_file
wget https://raw.githubusercontent.com/dhruvvyas90/qemu-rpi-kernel/master/tools/config_ip_tables

#Setup cross compilation
cat >> .config << EOF
CONFIG_CROSS_COMPILE="arm-linux-gnueabihf"
EOF

#Inject QEMU configuration
cat ./config_file >> .config
cat ./config_ip_tables >> .config
```

4. Compile the kernel:

```
make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- menuconfig
make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- bzImage dtbs
```

5. Change the path of the compiled binaries:

```
cd ..
cp linux/arch/arm/boot/zImage ./kernel7.img
cp linux/arch/arm/boot/dts/versatile-pb.dtb ./versatile-pb.dtb
cp linux/vmlinux ./vmlinux
```

## 4. Start QEMU

Start QEMU with command-line interface mode (recommend):

```
sudo qemu-system-arm                                       \
    -M versatilepb                                         \
    -cpu arm1176                                           \
    -m 256                                                 \
    -hda 2018-11-13-raspbian-stretch.img                   \
    -kernel kernel7.img                                    \
    -dtb versatile-pb.dtb                                  \
    -net nic                                               \
    -net user,hostfwd=tcp::5022-:22                        \
    --append "rw console=ttyAMA root=/dev/sda2 panic=1"    \
    -nographic                                             \
    -no-reboot
```

Start QEMU with graphical user interface mode:

```
sudo qemu-system-arm                      \
  -M versatilepb                          \
  -cpu arm1176                            \
  -m 256                                  \
  -hda 2018-11-13-raspbian-stretch.img    \
  -kernel kernel7.img                     \
  -dtb versatile-pb.dtb                   \
  -net nic                                \
  -net user,hostfwd=tcp::5022-:22         \
  --append "root=/dev/sda2 panic=1"       \
  -no-reboot
```

The default username and password of the raspbian are **pi** and **raspberry**.
