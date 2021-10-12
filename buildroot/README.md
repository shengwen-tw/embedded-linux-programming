# Customize embedded Linux distribution with Buildroot

1. Download buildroot

```
git clone git://git.busybox.net/buildroot
cd buildroot
```

2. List all supported boards

```
make list-defconfigs
```

3. Initialize the configuration with 64bits Raspberry Pi 4

```
make raspberrypi4_64_defconfig
```

4. Start menuconfig

```
make menuconfig
```

5. Start building the system

```
make -j16
```
