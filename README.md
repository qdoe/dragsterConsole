# Dragster Console

This is a console application intended to read inputs sent to an atari running Dragster (1980) and output the fastest times possible using the inputs given, taking into account different frame rules.

## Caveat

personally, I think its best function is as an attempt counter; it's too inaccurate to really rely on as a real indicator for its originally intended purpose.  I think the inaccuracy comes mainly from being unable to sync when I am reading the input with when the atari is reading it, but there also could be some bugs in my code for simulating the game as well, I don't know.

## Setup

### Hardware

(this section will be fairly brief for now)

basically, the way atari controllers work, each direction, and the button have separate wires.  you'll want to cut a controller extension cable and splice each of the wires for the inputs (and GND) so that you can attach these to the pins on a teensy (I used 3.5, idk if there'd be any difference for other versions).

### Software

load teensy/atarireader.ino onto the teensy, then build and run the rest of it.  might need to update the L"COM3" in TeensyReader.cpp depending on what device the teensy is read as, but it should just work after.
