# One-Hot Conversion: Towards Faster Table-based A2B Conversion

This is the code supporting the paper: One-Hot Conversion: Towards Faster Table-based A2B Conversion [link](https://eprint.iacr.org/2022/1099).

## Instructions

The build flow for this repository is based on [libopencm](https://github.com/libopencm3/libopencm3) and the [libopencm3-template repository](https://github.com/libopencm3/libopencm3-template). It also takes away some elements from [PQM4](https://github.com/mupq/pqm4). The code targets the `STM32F407G-DISC1` board, but a `make` target is also available for executing on a host PC.

Being able to run the code in those repositories is a prerequisite for this repository. In particular, it is necessary to install/setup:

* The `arm-none-eabi-gcc` toolchain
* `stlink`
* `Python3` with `pyserial`

It is also necessary to initialize `libopencm`:

* `git submodule update --init libopencm3`
* `make -C libopencm3`

## Makefile

The code in this repository can be built with `make` and flashed to the board with `make flash`. The serial output can be retrieved with `make screen`, which should be setup before flashing the binary.

There are a few optional flags that can be selected in the `Makefile`:

* Profiling can be enabled: `{PROFILE_TOP_CYCLES, PROFILE_TOP_RAND, PROFILE_STEP_CYCLES, PROFILE_STEP_RAND}`.

  * `PROFILE_x_CYCLES` profiles the number of cycles for either the total execution (`x=TOP`) or the individual steps (`x=STEP`).

  * `PROFILE_x_RAND` profiles the requested number of random bytes for either the total execution (`x=TOP`) or the individual steps (`x=STEP`).

* For the A2B conversion:

  * The A2B algorithm can be selected: `{CLASSICAL, ROTATE, TABLE, CLASSICALNB}`

  * The number of bits of the input can be configured: `{BITS=x}`

* For the masked comparison:

  * The scheme can be selected: `{SABER, KYBER}`

  * The masked comparison scheme can be selected: `{GF, ARITH, SIMPLE, ROTATE, HYBRID}`

* The number of shares can be configured: `{NSHARES=x}`

* The number of tests can be configured: `{NTESTS=x}`

Additionally, it is possible to compile the code for execution on a host PC by setting `{PLATFORM=host}`. This also enables the `-DDEBUG` flag, which adds debugging statements to the code execution within the routines. The host executable can then be run with `make run`, which can be used for testing purposes.

## Acknowledgements

I would like to thank Michiel Van Beirendonck for his help in the setup of the PQM4 scripts.
