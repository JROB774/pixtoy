# pixtoy

![build](https://github.com/jrob774/pixtoy/actions/workflows/build.yaml/badge.svg)
![deploy](https://github.com/jrob774/pixtoy/actions/workflows/deploy.yaml/badge.svg)

PIXTOY is a fun and simple web toy for creating miniature pixelated graphics
effects using the Lua programming language. The app exposes a small set of
simple primitive drawing functions, as well as some other utility functions
(maths, bitwise ops, etc.) to allow the creation of the effects.

You can see an example of a PIXTOY program by going to the [main page here](https://www.nksoftworks.com/pixtoy/).

If you're interested in trying it out follow the link above! There is also a
[helpful documentation page](https://www.nksoftworks.com/pixtoy/help.html) that
consists of both a getting started section and a full reference for all of the
API functions and global variables.

## Building

To build PIXTOY locally is quite simple, all of the dependencies are contained
within the repository as submodules. Just run these commands from a terminal:

```
git clone https://github.com/JROB774/pixtoy.git --recursive
cd pixtoy
build setup
build
```

There are a few different commands the build script can perform:

```
build setup  :: setup the emscripten environment for compiling on the web
build clean  :: clean the output binary folder 'web/app/' of files
build server :: setup a localhost server for pixtoy (requires Python3)
build        :: build the actual PIXTOY C web app for local execution
```

## Attribution

The palette used in PIXTOY is the **[Sweetie 16](https://lospec.com/palette-list/sweetie-16)** palette, developed by **[Grafxkid](https://grafxkid.tumblr.com/)**.

## License

The project's code is available under the **[MIT License](https://github.com/JROB774/pixtoy/blob/master/LICENSE)**, Copyright (C) Joshua Robertson 2022.
