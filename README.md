# OmegaWTK (Omega Widget ToolKit)
 A cross-platform UI written in c++

## Why Re-Invent The Wheel?

Many of the current cross-platform user interface engines such as QT or even Chrome either use a native to web based language bridge such as JavaScript or use outdated 3D pipelines, causing performance delays and an incoherent interface. 

OmegaWTK is a native object oriented interface written entirely in native code that is designed to be not only easy to use but also maxmise performance for the programmer.

---

## Want to Contribute?

Omega Graphics is a growing open source project always looking for new members and contributions.
Fill out the quick form here:

https://forms.gle/3akUSgjMj3txz2jD8

And you will be given access to our libraries.




##### Requirements:

> Clone the [`omega-graphics/autom`](https://github.com/omega-graphics/autom) repo and follow the instructions listed in the README


1. Clone the repo
```sh
autom-clone https://github.com/omega-graphics/omega-wtk-project ./wtk
```
2. Configure the build
```sh
autom --mode gn --out out
```

3. Build the project
```sh
ninja -C out
```
<!-- 
4. Install
```sh
autom-install --prefix ~/OmegaWTK ./out
``` -->


### License

Apache 2.0

See [LICENSE](LICENSE)