# Photoshop
## Build
### Prerequisites
* GNU Make
* GCC
### Steps
Simply execute `make`. Binary will be inside generated folder `build`.
## Usage
**Photoshop** requires 2 arguments:
1. `--input input.tga` - input TGA image.
2. `--output output.tga` - output TGA image.

After executing you are ready to write commands. **Photoshop** supports 8 commands:
1. `resize w,h` - resize an image to width(w) and height(h).
2. `crop x,y,w,h` - crop an square inside image from x,y origins with height(h) and width(w) dimensions.
3. `copy x1,y1,w,h x2,y2` - copy pixels from x1,y1 origins to x2,y2 origins with height(h) and width(w) dimensions.
4. `move x,y` - move all pixels by x,y.
5. `bw` - convert image to black and white.
6. `info` - print info about an image.
7. `save` - save image to file specified in `--output` argument.
8. `exit` - executes `save` command and end the program.