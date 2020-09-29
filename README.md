# umbrel-details-gui

![Example](example.png)

A gui version of umbrel-details. Only works on Raspberry Pis.

## Required packages to run this:

```
sudo apt install libqt5gui5 libqt5svg5 libqt5network5 -y
```

## Build

```
mkdir build
cd build
qmake ..
make -j$(nproc)
```

## Running

To run this on an Raspberry Pi with Umbrel OS, run
```
./umbrel-details -platform linuxfb
```