# Embedded Linux Programming

Some code collections for developing embedded linux programs.

## Virtual Raspberry Pi environment with Docker

**For developing kernel module, it is recommended to test in the virtual environment to prevent the damage of the system.**

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
sudo docker run -it -p 5900:5900 --name raspbian tommychap/raspberrypi:latest
```

2. Run the container created previously:

```
sudo docker start raspbian
sudo docker attach raspbian
```

4. Show all containers:

```
sudo docker ps
```

3. [Optional] To remove the container:
```
sudo docker container rm raspbian
```

## References

1. [The Linux Kernel Module Programming Guide](https://sysprog21.github.io/lkmpg/)

2. [Linux Daemon Writing HOWTO](http://www.netzmafia.de/skripten/unix/linux-daemon-howto.html)

3. [Linux Book Club](https://hackmd.io/@combo-tw/Linux-%E8%AE%80%E6%9B%B8%E6%9C%83)
