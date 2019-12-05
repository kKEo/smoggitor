

Linux:

Install
``` 
curl -fsSL https://raw.githubusercontent.com/platformio/platformio-core/master/scripts/99-platformio-udev.rules | sudo tee /etc/udev/rules.d/99-platformio-udev.rules
```

```
sudo service udev restart

```
sudo usermod -a -G dialout $USER
sudo usermod -a -G plugdev $USER
```



$ platformio lib install 13 1513 6265

13 -
1513 - OLED
6265 - PMS7003

