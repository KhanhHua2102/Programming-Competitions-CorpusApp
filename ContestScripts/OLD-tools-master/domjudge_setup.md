# DOMjudge Setup

[Last Update: November 2021]

[Estimated Time: 15 minutes]

These are instructions for getting DOMJudge working on Digital Ocean. Note:
this page is self-contained. You shouldn't have to leave it for more info. If
you do, please add missing information.

## DOMserver

### Step 1: Get a Fresh VM

- Start a fresh Ubuntu or Debian VM on Digital Ocean.
- You will need a lot of CPUs (32+) for a contest with 75+ teams. 16+ is
  enough for 50+ teams.

### Step 2: Install Docker

External resource
[here](https://www.digitalocean.com/community/tutorials/how-to-install-and-use-docker-on-ubuntu-20-04)
if needed.

```
sudo apt update
sudo apt install docker.io
```

### Step 3: Install DOMserver

External resource [here](https://hub.docker.com/r/domjudge/domserver/) if
needed.

- Run MariaDB

```
docker run -it --name dj-mariadb --volume mariadb-datavolume:/var/lib/mysql \
    --restart always --detach -p 13306:3306 -e MYSQL_ROOT_PASSWORD=rootpw \
    -e MYSQL_USER=domjudge -e MYSQL_PASSWORD=djpw -e MYSQL_DATABASE=domjudge \
    mariadb:latest --max-connections=1000
```

- Wait... It should take ~20 seconds for the database to be set up after the download is complete...

- Run the DOMserver. NOTE: Change `7.3.3` to whatever DOMjudge version you
  want.

```
docker run --link dj-mariadb:mariadb -it --detach --restart always \
    -v domserver-datavolume:/opt/domjudge -e MYSQL_HOST=mariadb \
    -e MYSQL_USER=domjudge -e MYSQL_DATABASE=domjudge -e MYSQL_PASSWORD=djpw \
    -e MYSQL_ROOT_PASSWORD=rootpw -e CONTAINER_TIMEZONE=Australia/Sydney \
    -p 80:80 --name domserver domjudge/domserver:7.3.3
```

- Wait... It should take ~20 seconds for the domserver to set up after the download is complete...

- Get the admin password. You should be able to access the UI. Username: `admin` Password can be found with this:

```
docker exec -it domserver cat /opt/domjudge/domserver/etc/initial_admin_password.secret
```

- Get the judgehost password. You'll need this for later. Copy it somewhere.

```
docker exec -it domserver cat /opt/domjudge/domserver/etc/restapi.secret
```

- DOMserver is set up!

## Judgehost

### Step 1: Get a Fresh VM

- Start a fresh Ubuntu or Debian VM on Digital Ocean.

- You will need >2 GB of RAM (and at least 2 cores, but anything more than
  2 is useless unless we put multiple judgehosts on one machine later.
  The judgehost is bound to a single CPU, so they really don't help).

### Step 2: Install Docker

Same as above.

### Step 3: Enable cgroups

External resource
[here](https://www.domjudge.org/docs/manual/main/install-judgehost.html#linux-control-groups)
if needed.

- Add `quiet cgroup_enable=memory swapaccount=1` to `GRUB_CMDLINE_LINUX_DEFAULT`

1.  On Ubuntu (as of today), you do this by:

```
vim /etc/default/grub.d/50-cloudimg-settings.cfg
```

```
# In the file...
GRUB_CMDLINE_LINUX_DEFAULT="[WHATEVER IS ALREADY THERE] quiet cgroup_enable=memory swapaccount=1"
```

2.  On Debian (as of today), you do this by:

```
vim /etc/default/grub
```

```
# In the file...
GRUB_CMDLINE_LINUX_DEFAULT="[WHATEVER IS ALREADY THERE] quiet cgroup_enable=memory swapaccount=1 systemd.unified_cgroup_hierarchy=0"
```

- Update grub

```
update-grub
```

- Reboot

```
reboot
```

Done! cgroups is set up!

### Step 4: Install Judgehost

External resource [here](https://hub.docker.com/r/domjudge/judgehost/) if needed.

- Run the judgehost. Make sure to add in the Judgehost's password you got from above.
  - Make sure `7.3.3` matches the version you did for DOMserver.
  - `BASEURL` is the URL or IP where to find the domserver
  - `hostname` is what this shows up as on DOMjudge's UI. Unfortunately, with this
    setup, it will be the same for all machines (with a different number appended).

```
docker run -it --privileged --name judgehost-0 --detach --restart always \
    -v /sys/fs/cgroup:/sys/fs/cgroup:ro -v chroot-datavolume:/chroot \
    -v domserver-datavolume:/opt/domjudge --hostname ${HOSTNAME} -e DAEMON_ID=0 \
    -e CONTAINER_TIMEZONE=Australia/Sydney -e DOMSERVER_BASEURL=159.89.208.8/ \
    -e JUDGEDAEMON_PASSWORD=[INSERT_JUDGEHOST_PASSWORD_HERE] \
    domjudge/judgehost:7.3.3
```

- Wait... It should take ~20 seconds for the domserver to set up after the download is complete...

- At this point, only C++ works. Let's get Java and Python working!

### Step 5: Fix the Judgehost for Java/Python

- Open `bash` terminal inside Docker:

```
docker exec -it judgehost-0 bash
```

- Upgrade the Judgehost's packages, and install missing packages.

```
apt update
apt upgrade
apt install lsb-release nano
```

- One line needs fixing in the `chroot` script...

```
nano /opt/domjudge/judgehost/bin/dj_make_chroot
```

- Look for these lines (3/4 down the page). This is what they _should_ look like (change
  `/updates` to `-security` in the last line). You are trying to fix
  [this error](https://the-codeslinger.com/2020/03/26/debian-bullseye-the-repository-does-not-have-a-release-file/).

```
# $RELEASE
deb $DEBMIRROR                  $RELEASE                main
${DISABLE_SECURITY}deb http://security.debian.org       $RELEASE-security       main
```

- Recreate the `chroot`:

```
/opt/domjudge/judgehost/bin/dj_make_chroot
```

- Exit out of the Docker's `bash`:

```
exit
```

- DONE! You should now be able to have Java and Python working!
