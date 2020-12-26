This is now maintained at https://github.com/UmbrelOS-Tweaks/umbrel-gui.

# GUI for Umbrel details

![Example](example.png)

A gui version of umbrel-details. Only works on Raspberry Pis with Umbrel OS installed.

## Required packages to build this:

```
sudo apt install libqt5gui5-dev libqt5svg5-dev libqt5network5-dev cmake make gcc g++ qtwebengine5-dev -y
```

## Build

```
mkdir build
cd build
cmake ..
make -j$(nproc)
```

## Running

To run this on an Raspberry Pi with Umbrel OS, run
```
./umbrel-details
```

## License

The umbrel logo is Copyright 2020 Umbrel and is licensed under the terms and conditions of the MIT License.
