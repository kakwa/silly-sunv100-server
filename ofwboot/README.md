# Custom ofwboot

`ofwboot` build with tftp bootstrap to directly get a kernel from tftp

## Prerequisites

- bmake
- SPARC64 cross toolchain: `gcc-sparc64-linux-gnu` and `binutils-sparc64-linux-gnu`

Example (Debian/Ubuntu):

```bash
sudo apt-get update -y
sudo apt-get install -y bmake gcc-sparc64-linux-gnu binutils-sparc64-linux-gnu
```

## Build

From this directory:

```bash
CC=sparc64-linux-gnu-gcc LD=sparc64-linux-gnu-ld bmake
```

## Clean

```bash
bmake clean
```

## Test OFW version

```bash
CC=sparc64-linux-gnu-gcc LD=sparc64-linux-gnu-ld bmake -DKAKWAOFWTEST
```
