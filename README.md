# Zenzebu Engine

Zenzebu is a WIP open-source game engine developed primarily by [@nullium21](https://github.com/nullium21).
As of now, it doesn't have pretty much any features, but 3D rendering is [being developed](https://github.com/nullium21/zenzebu/tree/mesh-rendering).

## Getting the source

Regular `git clone` will not suffice for getting the code, because we have a few submodules in here, so this:
```bash
git clone --recursive https://github.com/nullium21/zenzebu
```
or this will work:
```bash
git clone https://github.com/nullium21/zenzebu
cd zenzebu
git submodule init --update
```

## Building

Zenzebu uses [Premake](https://premake.github.io) as the main build system, meaning you can generate Makefiles, Visual Studio projects/solutions, and more. It is currently only tested with Clang as the compiler, so, to generate Makefiles, run this command:
```bash
premake5 gmake2 --cc=clang
```
If you're feeling risky, though, you can omit the `--cc=clang` flag and try compiling with GCC -- this *should* work.

## Running

There is no default runnable project, but there are multiple [examples](https://github.com/nullium21/zenzebu/tree/master/examples) demonstrating some of the features. To compile them all, run `make examples`, and to compile only one, run `make ex.ecs`, where `ecs` is the name of it.

## Contributing?

If you feel something's missing (actually, a lot of stuff is, but we don't talk about that here), you can create an issue, or, what's even better, a pull request ;)
