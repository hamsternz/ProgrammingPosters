COPTS=-Wall -pedantic -lm -O4

all: images thumbs

images : output/ball.jpg output/mandelbrot.jpg  output/planet.jpg output/pulsar.jpg output/sinc.jpg output/torus.jpg output/waggle.jpg

thumbs : thumbs/ball.jpg thumbs/mandelbrot.jpg  thumbs/planet.jpg thumbs/pulsar.jpg thumbs/sinc.jpg thumbs/torus.jpg thumbs/waggle.jpg

texture/bluemarble.ppm :
	# You need to download the blue marble from:
	# https://visibleearth.nasa.gov/images/57735/the-blue-marble-land-surface-ocean-color-sea-ice-and-clouds
	wget https://eoimages.gsfc.nasa.gov/images/imagerecords/57000/57735/land_ocean_ice_cloud_2048.jpg
	jpegtopnm < land_ocean_ice_cloud_2048.jpg > $@

output/waggle.jpg : output/waggle.ppm
	cat $< | ppmtojpeg > $@

thumbs/waggle.jpg : output/waggle.ppm
	cat $< | pnmscale 0.10 | ppmtojpeg > $@

output/torus.jpg : output/torus.ppm
	cat $< | ppmtojpeg > $@

thumbs/torus.jpg : output/torus.ppm
	cat $< | pnmscale 0.10 | ppmtojpeg > $@

output/pulsar.jpg : output/pulsar.ppm
	cat $< | ppmtojpeg > output/pulsar.jpg

thumbs/pulsar.jpg : output/pulsar.ppm
	cat $< | pnmscale 0.10 | ppmtojpeg > $@

output/sinc.jpg : output/sinc.ppm
	cat $< | ppmtojpeg > $@

thumbs/sinc.jpg : output/sinc.ppm
	cat $< | pnmscale 0.10 | ppmtojpeg > $@

output/ball.jpg : output/ball.ppm
	cat $< | ppmtojpeg > $@

thumbs/ball.jpg : output/ball.ppm
	cat $< | pnmscale 0.10 | ppmtojpeg > $@

output/mandelbrot.jpg : output/mandelbrot.ppm 
	cat $< | ppmtojpeg > $@

thumbs/mandelbrot.jpg : output/mandelbrot.ppm 
	cat $< | pnmscale 0.10 | ppmtojpeg > $@

output/planet.jpg : output/planet.ppm
	cat $< | ppmtojpeg > $@

thumbs/planet.jpg : output/planet.ppm
	cat $< | pnmscale 0.10 | ppmtojpeg > $@

output/waggle.ppm : bin/waggle bin/make_poster 
	bin/waggle build/img.ppm && bin/make_poster src/waggle.c build/img.ppm output/waggle.ppm

output/torus.ppm : bin/torus bin/make_poster 
	bin/torus build/img.ppm && bin/make_poster src/torus.c build/img.ppm output/torus.ppm

output/pulsar.ppm : bin/pulsar bin/make_poster
	bin/pulsar build/img.ppm && bin/make_poster src/pulsar.c build/img.ppm output/pulsar.ppm

output/sinc.ppm : bin/sinc bin/make_poster
	bin/sinc build/img.ppm && bin/make_poster src/sinc.c build/img.ppm  output/sinc.ppm

output/ball.ppm : bin/ball bin/make_poster
	bin/ball build/img.ppm && bin/make_poster src/ball.c build/img.ppm output/ball.ppm

output/mandelbrot.ppm : bin/mandelbrot bin/make_poster
	bin/mandelbrot build/img.ppm && bin/make_poster src/mandelbrot.c build/img.ppm output/mandelbrot.ppm

output/planet.ppm : bin/planet bin/make_poster texture/bluemarble.ppm
	bin/planet build/img.ppm && bin/make_poster src/planet.c build/img.ppm output/planet.ppm

bin/waggle: src/waggle.c
	gcc -o $@ $< $(COPTS)

bin/torus: src/torus.c
	gcc -o $@ $< $(COPTS)

bin/ball: src/ball.c
	gcc -o $@ $< $(COPTS)

bin/sinc: src/sinc.c
	gcc -o $@ $< $(COPTS)

bin/mandelbrot: src/mandelbrot.c
	gcc -o $@ $< $(COPTS)

bin/pulsar: src/pulsar.c
	gcc -o $@ $< $(COPTS)

bin/planet: src/planet.c
	gcc -o $@ $< $(COPTS)

bin/make_poster: src/make_poster.c
	gcc -o $@ $< $(COPTS)

