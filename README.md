# Eovim

Eovim is the Enlightened Neovim. That's just an [EFL][1] GUI client for
[Neovim][2].

## Status

Eovim is still at an early alpha stage. However, it should be possible to use
it for developping daily, as most of the Neovim interfaces have been
implemented.

## Installation

Eovim requires the following components to be installed on your system before
you can start hacking:

- [EFL][1]: this framework of libraries is packaged in most of the GNU/Linux
  distributions and on macOS.
- [msgpack-c][3]: this serialization library is not widely packaged, but is
  mandatory to communicate with Neovim. You are advised to run the script
  `scripts/get-msgpack.sh` to install msgpack. This will retrieve and compile
  a static version of msgpack that `eovim` can work with.
- [Neovim][2] (version 0.2.1 or greater),
- CMake.

Then this is straightforward CMake build:

```bash
mkdir -p build && cd build
cmake -G "Unix Makefiles" ..
make
make install # Possibly as root (i.e. via sudo)
eovim
```

## Usage

```bash
eovim [files...]
```

For now, `eovim` can be run without any argument. This is equivalent to run
Neovim without any file. You can pass files as arguments to `eovim`, they will
be opened at startup the same way Neovim does.

When `eovim` starts, it spawns an instance of Neovim. If it happens that `nvim`
is not in your `PATH` or if you want to use an alterate binary of Neovim, you
can feed it to `eovim` with the option `--nvim`.


## Hacking

Eovim uses some environment variables that can influence its runtime. Some are
directly inherited from the [EFL][1] framework, others are eovim-specific:
- `EINA_LOG_BACKTRACE` set it to an integer to get run-time backtraces.
- `EINA_LOG_LEVELS` set it to "eovim:INT" where _INT_ is the log level.
- `EOVIM_IN_TREE` set it to non-zero to load files from the build directory
   instead of the installation directory.

To develop/debug, a typical use is to run `eovim` like this (from the build
directory):

```bash
env EOVIM_IN_TREE=1 EINA_LOG_BACKTRACE=0 EINA_LOG_LEVELS="eovim:3" ./eovim
```

# License

Eovim is MIT-licensed. See the `LICENSE` file for details. Files in
`data/themes/img` have been taken from [terminology][4] or the [EFL][1] and are
not original creations.

[1]: https://www.enlightenment.org
[2]: https://neovim.io
[3]: https://github.com/msgpack/msgpack-c
[4]: https://www.enlightenment.org/about-terminology
