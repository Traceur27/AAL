# Flooding algorithm
Comparision of two algorithms(naive and more optimal) which are computing the number of flooded fields on provided 3D map.

## Usage
Requirements:
```bash
sudo apt-get install libglew-dev libsoil-dev freeglut3-dev
```
Run example:
```bash
git clone https://github.com/kczarnota/Flood
cd Flood
mkdir build
cd build
cmake ..
make
./Generator 8 8 4 r
./Flood data.txt u
```
Controls:
* use arrow keys to rotate the board
* use 'q' to show map before/after flooding
* use '+', '-' to zoom in/out

## Screenshot
![](screenshots/pacman.png?raw=true)