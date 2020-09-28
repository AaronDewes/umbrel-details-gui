# umbrel-details-gui

A gui version of umbrel-details. Only works on Raspberry Pis.

## Required packages to run this:

```
sudo apt install libqt5gui5 libqt5svg5 libqt5network5 -y
```

## Build

```
qmake
make -j3
./umbrel-details -platform eglfs
```
