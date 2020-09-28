# umbrel-details-gui

A gui version of umbrel-details. Only works on Raspberry Pis.

## Required packages to run this:

```
sudo apt install libqt5waylandcompositor5 libqt5gui5 libqt5svg5 -y
```

## Build

```
qmake
make -j3
./umbrel-details -platform eglfs
```
