# Chimes
A little useful C++ algorithm library.
____
### Building Chimes

#### Dependence

- Eigen3 for Optimization

#### Compile options

*USE_OPTIMIZATION*: for enable the optimization module

#### Makefile builds (Linux, other Unixes, and Mac)

```
git clone https://github.com/BKHao/Chimes
cd Chimes
mkdir build && cd build
cmake ..
make -j8
make install
```

After installed, using FindChimes in your project please.

#### MSVC on Windows

```
git clone https://github.com/BKHao/Chimes
```
Open cmake-gui

```
Where is the source code: Chimes

Where to build the binaries: Chimes/build
```

note: check the location of dependencies and install. It is recommended to use vcpkg to add dependencies.

Configure->Generate->Open Project

ALL_BUILD->INSTALL

### Test

The example and data is in 'test'. Include Chimes in your project when testing and using it.


### Development

27/04/2022 Start.

29/04/2022 Complete steepest descent method in Optimization.

04/05/2022 Complete a simple memory pool, which contains three levels of cache and supports concurrency(very crude).