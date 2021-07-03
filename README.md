# OmegaWTK (Omega-Widget Tool Kit)
 A cross-platform widget toolkit written in c++

### Build 

>NOTE: Please clone the `omega-graphics/autom` repo and build the contents of that repo before continung
1. Clone the repo
```sh
clone https://github.com/omega-graphics/omega-wtk-project ./wtk
```
2. Configure the build
```sh
autom --mode gn --out out
```

3. Build the project
```sh
ninja -C out
```

4. Install
```sh
autom-install --prefix=$USERS_PATH/OmegaWTK ./out
```