
## Install dependency
libCacheSim uses [camke](https://cmake.org/) build system and has a few dependencies: 
[glib](https://developer.gnome.org/glib/)
[tcmalloc](https://github.com/google/tcmalloc), 
[zstd](https://github.com/facebook/zstd).


### Install dependency on Ubuntu
#### Install glib, tcmalloc and cmake
```bash
sudo apt install libglib2.0-dev libgoogle-perftools-dev cmake
```

#### Install zstd
zstd must be installed from source
```bash
wget https://github.com/facebook/zstd/releases/download/v1.5.0/zstd-1.5.0.tar.gz
tar xvf zstd-1.5.0.tar.gz
pushd zstd-1.5.0/build/cmake/
mkdir _build && cd _build/
cmake .. && make -j
sudo make install
popd
```

#### Install XGBoost [Optional]
```bash
git clone --recursive https://github.com/dmlc/xgboost
pushd xgboost
mkdir _build && cd _build
cmake .. && make -j
sudo make install
popd
```

#### Install LightGBM [Optional]
```bash
git clone --recursive https://github.com/microsoft/LightGBM
pushd LightGBM
mkdir _build && cd _build
cmake .. && make -j
sudo make install
popd
```


### Install dependency on Mac
using [homebrew](https://brew.sh/) as an example
```bash
brew install cmake glib google-perftools
```
And you need to install the rest of the dependencies manually.


