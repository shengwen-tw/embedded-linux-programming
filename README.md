# Embedded Linux Programming

Some code collections for developing embedded linux programs.

## Docker virtual environment

**It is recommended to test kernel modules in the virtual environment to prevent damage on the system.**

0. Prerequisites

```
sudo apt-get update
sudo apt-get install apt-transport-https ca-certificates curl gnupg lsb-release

curl -fsSL https://download.docker.com/linux/ubuntu/gpg | sudo gpg --dearmor -o /usr/share/keyrings/docker-archive-keyring.gpg

echo "deb [arch=$(dpkg --print-architecture) signed-by=/usr/share/keyrings/docker-archive-keyring.gpg] https://download.docker.com/linux/ubuntu $(lsb_release -cs) stable" | sudo tee /etc/apt/sources.list.d/docker.list > /dev/null

sudo apt-get update
sudo apt-get install docker-ce docker-ce-cli containerd.io
```

1. Create new container:

```
sudo docker run --privileged -it -p 5900:5900 --name ubuntu-xenial tommychap/raspberrypi:latest
```

2. Run the container created previously:

```
sudo docker start ubuntu-xenial
sudo docker attach ubuntu-xenial
```
3. Install **sudo** command for docker image

```
apt-get update && apt-get -y install sudo
```

4. Show all containers:

```
sudo docker ps
```

5. [Optional] To remove the container:
```
sudo docker container rm ubuntu-xenial
```

## References

1. [The Linux Kernel Module Programming Guide](https://sysprog21.github.io/lkmpg/)

2. [Linux Daemon Writing HOWTO](http://www.netzmafia.de/skripten/unix/linux-daemon-howto.html)

3. [Linux Book Club](https://hackmd.io/@combo-tw/Linux-%E8%AE%80%E6%9B%B8%E6%9C%83)
