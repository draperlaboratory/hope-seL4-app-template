# hope-seL4-app-template - seL4 runtime bootstrapper

Minimal seL4 setup that allows an arbitrary program with an `isp_main()` function
to be dropped in and run. Uses seL4 master and a few CMake files from seL4test. This has only been tested with a simple hello world program, so there may be issues with using this template for more complex programs.

Note that this is different than the official [sel4 runtime](https://github.com/draperlaboratory/hope-sel4runtime), which appears to still be a WIP.

## Usage

* This repository is primarily intended to be automatically utilized by the
`hope-tools` ISP runtime. See
[the runtime README](https://github.com/draperlaboratory/hope-tools/blob/master/runtime/README.md)
for details. 

## Manual Usage (no runtime)

It is recommended to make a separate copy of `hope-sel4` for each app you wish to run within seL4.

* Run `setup.sh` to clone seL4 repos and put them in the right places.
* Build arbitrary program into a static library named `target.a` and place it
in `projects/bootstrap_main/src/`
* Build seL4 + arbitrary program with
`mkdir build; cd build; ../init-build.sh -DPLATFORM=spike -DRISCV32=true; ninja`
* Simulate with
`./simulate -b /path/to/stock/qemu-system-riscv32 -M virt`. Note that seL4 currently does not run
on the ISP simulator.

