# Cobalt Overview

Cobalt is a lightweight application container that is compatible with a subset
of the W3C HTML5 specifications.

## Interesting Source Locations

All source locations are specified relative to `src/` (this directory).

  * `base/` - Chromium's base library. Contains common utilities, and a light platform
    abstraction.
  * `net/` - Chromium's net library. Contains most of a HTTP User-Agent, and
    HTTP server, a DIAL server, and several abstractions for networking
    primitives.
  * `media/` - Chromium's media library. Contains all the code that parses,
    processes, and manages buffers of video and audio data.
  * `cobalt/` - The home of all Cobalt application code.
    * `cobalt/build/` - The core build generation system, gyp_cobalt, and
      configurations for supported platforms.
  * `starboard/` - Cobalt's porting layer. Still an early
    work-in-progress. Check Starboard's README.md for more detailed infromation.

## Building and Running the Code

Here's a quick and dirty guide to get to build the code on Linux.

  1. Install the provided `depot_tools.tar.gz` file into your favorite
     directory. It has been slightly modified from Chromium's depot_tools.
  2. Add that directory to the end of your `$PATH`.
  3. Ensure you have these packages installed: `sudo apt-get install
     libgles2-mesa-dev libpulse-dev libavformat-dev libavresample-dev`
  4. Remove bison-3 and install bison-2.7, or just make sure that bison-2.7 is
     at the beginning of your `$PATH`.
  5. (From this directory) run `cobalt/build/gyp_cobalt -C Debug linux`
  6. If you get a "clang not found" error, add the path to Cobalt's clang to
     your `$PATH` and rerun `gyp_cobalt` as above. For example:
     `/path/to/cobalt/src/third_party/llvm-build/Release+Asserts/bin`
  7. Run `ninja -C out/Linux_Debug cobalt`
  8. Run `out/Linux_Debug/cobalt --url=<url>`
