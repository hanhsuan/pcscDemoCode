# pcscDemoCode
Demo code for using PC/SC to communicate with smart card

This demo code will read serial number from smartcard and could be compiled on Windows, MacOS and Linux by cmake.

## compile
### Windows
Windows only provide x86 version winscard.dll, therefore, you have to install x86 SDK first.

Generating compiler configuration
```shell
cmake -A Win32 -S . -B build -DCMAKE_BUILD_TYPE=Release
```

Compile by Visual Studio 2022
```shell
cmake --build build --target all package --config Release
```

Compile by github action
```shell
cmake --build build --target ALL_BUILD PACKAGE --config Release
```

### Linux
Install pcsclite and pcscd first

For debian-based
```shell
sudo apt install libpcsclite-dev pcscd
```

For redhat-based
```shell
sudo dnf install pcsc-lite-devel 
```

Generating compiler configuration
```shell
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
```

Compile
```shell
cmake --build build --target all package --config Release
```

### MacOS
Generating compiler configuration
```shell
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
```

Compile
```shell
cmake --build build --target all package --config Release
```

## Run demo
### Windows
```shell
./output/bin/demo
```

### Linux
Start pcscd daemon
```shell
sudo systemctl start pcscd
```

```shell
./output/bin/demo
```

### MacOS
```shell
./output/bin/demo
```
