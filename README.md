# ProgrammingPosters
C that that make nice posters of short C programs.

This generates A2 300D DPI images of programs that implement intresting graphics.

Hopefully these will encouage the viewer to look at the code and see how it runs.

## Rules for inclusion
- Must only use standard C libraries
- All code fits on the poster
- Code is sensibly indented
- Reasonable level of error checking
- No external dependancies

The 'pnmtojpeg' tool, part of netpbm, is used to convert the 'PNM P6' output files to a compressed JPG image.

(I bent the rules slightly for planet.jpg, allowing it to pull down a texture from NASA)

## Known Issues
- the Makefile is a mess

## Building
If you have gcc and netpmb installed you should be just able to type
    make
The images will be in the output directory

## Previews of posters
![Alt text](thumbs/ball.jpg?raw=true "Title")
![Alt text](thumbs/waggle.jpg?raw=true "Title")
![Alt text](thumbs/mandelbrot.jpg?raw=true "Title")
![Alt text](thumbs/planet.jpg?raw=true "Title")
![Alt text](thumbs/pulsar.jpg?raw=true "Title")
![Alt text](thumbs/sinc.jpg?raw=true "Title")
![Alt text](thumbs/torus.jpg?raw=true "Title")

