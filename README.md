String Art
==========

Converts grayscale input image to "string art," winding black thread around
pins arranged around a circular white canvas, upscaling the output image
resolution as described by [Birsak et. al.](doi:10.1111/cgf.13359).

See my [blog article](https://possiblywrong.wordpress.com/2022/01/22/string-art/)
for details.

Usage
-----

Command line arguments specify the grayscale input image filename,
upscaling block size, number of pins, and output image filename. Image
files are in Netpbm 8-bit grayscale binary "P5" format (comments in the
image file header are not supported).

For example, given a 512x512 image file `input.pgm`, the following will
create a 4096x4096 `output.pgm` image file using 256 pins:

```
string_art input.pgm 16 256 output.pgm
```

References
----------

1. Vrellis, Petros, A new way to knit (2016).
   [http://artof01.com/vrellis/works/knit.html](http://artof01.com/vrellis/works/knit.html)
2. Birsak, M., Rist, F., Wonka, P., & Musialski, P., String Art: Towards
   Computational Fabrication of String Images, *Computer Graphics Forum*,
   **37**(2) 2018, 263–274, [doi:10.1111/cgf.13359](doi:10.1111/cgf.13359)
