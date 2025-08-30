### Building ofwboot (cross-compiling on Debian/Ubuntu)

This `ofwboot` port builds with BSD make (`bmake`) and a SPARC64 cross GCC. It expects OpenBSD's standalone support library (`libsa`) to be available for headers and linking.

### Prerequisites

- bmake
- SPARC64 cross toolchain: `gcc-sparc64-linux-gnu` and `binutils-sparc64-linux-gnu`

Example (Debian/Ubuntu):

```bash
sudo apt-get update -y
sudo apt-get install -y bmake gcc-sparc64-linux-gnu binutils-sparc64-linux-gnu
```

Optional sanity checks:

```bash
bmake -V .MAKE.VERSION
sparc64-linux-gnu-gcc -v
```

### Build

From this directory:

```bash
CC=sparc64-linux-gnu-gcc bmake
```

To clean:

```bash
bmake clean
```

### About libsa (required)

This tree includes a minimal set of machine headers under `deps/include`, but it still requires OpenBSD's standalone library (`libsa`) for headers like `lib/libsa/stand.h` and for linking.

What you need to provide (you said you'll handle this):

- `libsa` headers visible under an include path so that `#include <lib/libsa/stand.h>` resolves.
- A `libsa` archive to link against (and zlib if needed), matching the interfaces used here.

Notes and tips:

- The Makefile uses `S` as the OpenBSD source root and adds `-I${S}` to include paths. By default it sets `S=${.CURDIR}/../../../..`, which likely does not point to an OpenBSD src tree in this repository layout. You can override it on the command line:

  ```bash
  CC=sparc64-linux-gnu-gcc bmake S=/path/to/openbsd/src
  ```

- If your `libsa` is installed/built elsewhere, you may also need to ensure the link step finds it by setting appropriate variables or augmenting `CPPFLAGS`/`LDFLAGS`/`LIBS` as needed. The link rule in the Makefile expects `${LIBSA}`/`${LIBSADIR}` and `${LIBZ}`/`${LIBZDIR}` as provided by OpenBSD's makefiles. When using system `bmake` on Linux, you might choose to hardcode or export those, or adjust the link rule.

