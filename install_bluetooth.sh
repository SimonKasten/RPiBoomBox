

#!/bin/bash

#The bluetooth device can appear as a number of things.
#Change the line '+ Class = 0x200414' below to one of the following classes;
# 0x200418 - Headphones
# 0x200414 - Loudspeaker (default)
# 0x200420 - Car Audio
# 0x200434 - Car audio, Loudspeaker
# 0x200438 - Car Audio, Headphones
#
# All of the above classes show the headphone icon on iOS when connected.
# If you wish to not have that icon then use one of the following;
# 0x20041C - Headphones, Portable Audio / Portable Audio
# 0x20043C - Headphones, Portable Audio, Car audio / Portable Audio, Car audio


read -p "Bluetooth device name: " BT_NAME

#--------------------------------------------------------------------
function tst {
    echo "===> Executing: $*"
    if ! $*; then
        echo "Exiting script due to error from: $*"
        exit 1
    fi	
}
#--------------------------------------------------------------------


apt-get update -y
apt-get upgrade -y

# Install Pulse Audio & Bluez
apt-get install bluez pulseaudio pulseaudio-module-bluetooth -y
# Install dbus for python
apt-get install python-dbus -y
# Install espeak
apt-get install -qy espeak

# Create users and priviliges for Bluez-Pulse Audio interaction - most should already exist
addgroup --system pulse
adduser --system --ingroup pulse --home /var/run/pulse pulse
addgroup --system pulse-access
adduser pulse audio
adduser root pulse-access
adduser pulse lp

read -p "Bluetooth device name: " BT_NAME

mkdir /home/pi/pyScripts
cp usr/local/bin/volume-watcher.py /usr/local/bin/volume-watcher.py
chmod +x /usr/local/bin/volume-watcher.py
cp lib/systemd/system/volume-watcher.service /lib/systemd/system/volume-watcher.service
systemctl enable volume-watcher
cd `dirname $0`

echo "PRETTY_HOSTNAME=$BT_NAME" >> /tmp/machine-info
sudo cp /tmp/machine-info /etc

sudo cp init.d/pulseaudio /etc/init.d
sudo chmod +x /etc/init.d/pulseaudio
sudo update-rc.d pulseaudio defaults

sudo cp init.d/bluetooth /etc/init.d
sudo chmod +x /etc/init.d/bluetooth
sudo update-rc.d bluetooth defaults

sudo cp init.d/bluetooth-agent /etc/init.d
sudo chmod +x /etc/init.d/bluetooth-agent
sudo update-rc.d bluetooth-agent defaults

sudo cp usr/local/bin/bluez-udev /usr/local/bin
sudo chmod 755 /usr/local/bin/bluez-udev

sudo cp usr/local/bin/simple-agent.autotrust /usr/local/bin
sudo chmod 755 /usr/local/bin/simple-agent.autotrust

sudo cp usr/local/bin/say.sh /usr/local/bin
sudo chmod 755 /usr/local/bin/say.sh

sudo cp usr/local/bin/bluezutils.py /usr/local/bin

sudo cp etc/pulse/daemon.conf /etc/pulse

sudo patch /boot/config.txt << EOT
@@ -54,3 +54,6 @@

 # Enable audio (loads snd_bcm2835)
 dtparam=audio=on
+
+# High Quality audio patch
+audio_pwm_mode=2
EOT

if [ -f /etc/udev/rules.d/99-com.rules ]; then

sudo patch /etc/udev/rules.d/99-com.rules << EOT
***************
*** 1 ****
--- 1,2 ----
  SUBSYSTEM=="input", GROUP="input", MODE="0660"
+ KERNEL=="input[0-9]*", RUN+="/usr/local/bin/bluez-udev"
EOT

else

sudo touch /etc/udev/rules.d/99-com.rules
sudo chmod 666 /etc/udev/rules.d/99-com.rules
sudo cat  << EOT > /etc/udev/rules.d/99-input.rules
SUBSYSTEM=="input", GROUP="input", MODE="0660"
KERNEL=="input[0-9]*", RUN+="/usr/local/bin/bluez-udev"
EOT

fi

tst sudo chmod 644 /etc/udev/rules.d/99-com.rules

sudo patch /etc/bluetooth/main.conf << EOT
***************
*** 7,8 ****
--- 7,9 ----
  #Name = %h-%d
+ Name = ${BT_NAME}

***************
*** 11,12 ****
--- 12,14 ----
  #Class = 0x000100
+ Class = 0x200414

***************
*** 15,17 ****
  # 0 = disable timer, i.e. stay discoverable forever
! #DiscoverableTimeout = 0

--- 17,19 ----
  # 0 = disable timer, i.e. stay discoverable forever
! DiscoverableTimeout = 0
EOT

sudo patch /etc/pulse/system.pa << EOT
***************
*** 23,25 ****
  .ifexists module-udev-detect.so
! load-module module-udev-detect
  .else
--- 23,26 ----
  .ifexists module-udev-detect.so
! #load-module module-udev-detect
! load-module module-udev-detect tsched=0
  .else
***************
*** 57 ****
--- 58,63 ----
  load-module module-position-event-sounds
+
+ ### Automatically load driver modules for Bluetooth hardware
+ .ifexists module-bluetooth-discover.so
+     load-module module-bluetooth-discover
+ .endif
EOT



sudo patch /etc/pulse/system.pa << EOT
***************
*** 4,5 ****
deb http://ftp.jp.debian.org/debian/ jessie-backports main contrib non-free
deb-src http://ftp.jp.debian.org/debian/ jessie-backports main contrib non-free
EOT





#sudo service bluetooth start &
#sudo service pulseaudio start &
#sudo service bluetooth-agent start &
# BT FIX
mkdir /etc/pulsebackup
cp /etc/pulse/* /etc/pulsebackup/
git clone --branch v6.0 https://github.com/pulseaudio/pulseaudio
apt-get install intltool -y
apt-get install libsndfile-dev -y
apt-get install libcap-dev -y
apt-get install libjson0-dev -y
cd /home/pi/pulseaudio
./bootstrap.sh
make
make install
ldconfig
cp /etc/pulsebackup/* /etc/pulse

sleep 5

echo "Done! You should reboot now"
