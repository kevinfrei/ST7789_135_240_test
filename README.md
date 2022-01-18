## ST7789 135x240 test

This is a testing repo for the ST7789 135x240 screen to validate that a PR fixes
an off-by-one issue. (News flash: My initial PR doesn't fix it fully!)

To try it out yourself, copy the contents of
[test.cpp](https://raw.githubusercontent.com/kevinfrei/ST7789_135_240_test/main/test.cpp)
into a new Arduino sketch, configure the `TFT_CS`, `TFT_DC`, and `TFT_RST` (and
optionally the `BACKLIGHT`) pin numbers and run it.

Correct operation should be a full bright green outline for all 4 orientations,
and _never_ a blue line anywhere (You may see the screen go fully blue
occasionally, but it should immediately go back to black)

This repo is an overly-complicated setup so I could easily test my own fork of
the fabulous
[Adafruit-ST7735-Library](https://github.com/kevinfrei/Adafruit-ST7735-Library).
(Seriously: [buy their products](https://shop.adafruit.com), just to support
their software. It's really, really good stuff).

### Overly Complicated Details

For [other reasons](https://github.com/kevinfrei/FreiKey), I made a
[tool](https://github.com/kevinfrei/Arduino2Make) a while back that spits out
Makefile includable stuff, so I can do `make -j` and compile & link an order of
magnitude or two faster than the silly Arduino Studio build system. (I'm quite
comfortable make sure I `#include` all the necessary files, Arduino, thanks
anyway). It's obviously faster on my Mac laptop & Mini. On my 12 core Windows
desktop? Yeah, there's just no contest. Besides the incredibly slow "let's check
to see if they forgot any #include files" step, the Arduino build is
single-threaded.
