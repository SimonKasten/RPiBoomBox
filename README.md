# RPiBoomBox
Use raspberry for a2dp-bluetooth music streaming

Main work was done by:
- Martin Engström (original BitBucket repository)
- Aden Beckitt (2017 Raspbian Jessie rewrite)
- Brett Reinhard (2017 Raspbian Jessie minimal rewrite, added features)


I did some fixes for the bluetooth-streaming (Volumio) installation that worked for me.


pulseaudio 7.12
pulseaudio -d
pavucontrol
ps -C pulseaudio
install.sh      


Versuch 1:                                                                   
siehe rpiSHScript-datei

bt_config bearbeitet wegen pulseaudio
pulseaudio 7.1 installiert 
bluez simple-agent anpassen
python 3.5 installieren 

ABKACKEN WEGEN STROM = USB HUB ?





evtl hilfreiche befehle:
systemctl status pulseaudio

sudo amixer cset numid=3 <n>
Where is 0=auto, 1=analog, 2=hdmi.
 By default it's set to auto which means HDMI if present,
 analog otherwise. Unlike changing video output,
 this change does not need rebooting.

pavucontrol
https://freedesktop.org/software/pulseaudio/pavucontrol/#download

pulseaudio pulseaudio7.1 installiert siehe 
https://raspberrypi.stackexchange.com/questions/53393/pulseaudio-6-0-on-raspbian-jessi

https://wiki.debian.org/PulseAudio#Installing_PulseAudio

Soundserver läuft? Prozess pulseaudio:
ps -C pulseaudio 

restart pulseaudio 
pulseaudio -k    # Beendet PulseAudio
pulseaudio -D # Neustart als daemon 
