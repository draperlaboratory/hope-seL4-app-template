# hope-seL4-app-template - seL4 runtime bootstrapper

Minimal seL4 setup that allows an arbitrary program with an `isp_main()` function
to be dropped in and run. Uses seL4 master and a few CMake files from seL4test.
This has only been tested with a simple hello world program, so there may be
issues with using this template for more complex programs.

Note that this is different than the official [sel4 runtime](https://github.com/draperlaboratory/hope-sel4runtime),
which appears to still be a WIP.

## Usage

This repository is primarily intended to be automatically utilized by the
`hope-tools` ISP runtime. See
[the runtime README](https://github.com/draperlaboratory/hope-tools/blob/master/runtime/README.md)
for general runtime details. 

seL4 specific runtime details:

Building:
* The application must have an `isp_main()` function instead of `main()`.
* The application being used with the runtime must have a makefile target called
`sel4-lib` that compiles the application into a static library called `target.a`. 
An example of such a target is in [common.mk](https://github.com/draperlaboratory/hope-applications/blob/sel4_support/hello-world/common.mk)
in the hello-world example program.
* `target.o` will get moved into the appropriate location in the template and
linked into the bootstrapper. 
* Currently, `sel4` and `stock_sel4` are equivalent since seL4 doesn't build
with clang, so both runtimes just use the stock GCC toolchain in the `stock-tools`
directory (see the runtime documentation for information about stock tools usage).

To actually carry out the build with the runtime:
* Set up an app with runtime-compatible makefiles, including a library-building
target.
* `cd /path/to/app`
* `isp_install_runtime [sel4, stock_sel4]`
* `make -f Makefile.[sel4, stock_sel4]` 
 
Running (These are the same steps as for simulating [seL4test on ISP](https://github.com/draperlaboratory/hope-sel4-riscv-notes/blob/master/README.md)):

Run with the validator using:

* `cd /path/to/app` (assuming you are not still in the same directory)
* `cd hope-seL4-app-template/riscv-build-spike-32`
* `isp_kernel osv.bare.main.none -o .` (there is not support for seL4 policies yet, so we generate a generic bare policy directory)
* `isp_run_app --policy none --simulator qemu --runtime sel4 --output ./sel4test-tagged  --debug images/sel4test-driver-image-riscv-spike -p osv.bare.main.none` (The generated policy directory is used to override the runtime from trying to use a nonexistent seL4 directory, but we need to specify `seL4` rather than `bare` as the runtime so that the right QEMU flags are used).

Run without the validator using:


* `cd /path/to/app` (assuming you are not still in the same directory)
* `cd hope-seL4-app-template/riscv-build-spike-32`

Then either run 

* `isp_run_app --policy none --simulator qemu --runtime sel4 --output ./sel4test-tagged-novalidator --debug images/bootstrap_main-image-riscv-spike -N`

or 

`./simulate -b /path/to/stock/qemu-system-riscv32 -M virt` to bypass the runtime


## Manual Usage (no runtime)

You must make a separate copy of `hope-sel4` for each app you wish to run within seL4.

* Run `setup.sh` to clone seL4 repos and put them in the right places.
* Build arbitrary program into a static library named `target.a` and place it
in `projects/bootstrap_main/src/`
* Build seL4 + arbitrary program with
`mkdir build; cd build; ../init-build.sh -DPLATFORM=spike -DRISCV32=true; ninja`
* Simulate with
`./simulate -b /path/to/stock/qemu-system-riscv32 -M virt`. Note that seL4 currently does not run
on the ISP simulator.

