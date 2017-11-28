# Raytracer

This is a program which does raytracing!
Recursively and everything!
Heck yeah!



## Compilation
Unpack the tar.
Then, run this command.

```bash
make
```

You will need a C++ compiler that supports C++14, which all the CS lab machines have.
Note that this will compile with `-Ofast -march=native -flto`, so the speed of compilation will be rather slow.
Sorry.

If you have troubles, try running `make clean` first.


## Running

Run this command:

```bash
./raytracer DRIVERFILE OUTFILE
```

Where `DRIVERFILE` is a path to a Driver File, as specified in the assignment.
This code will output how much progress it's made every second or so, as a percentage.
It will then create PPM-formatted image in `OUTFILE`.

## Custom Renderings
The two custom drivers that I created myself are located in the `custom_drivers/` folder, along with associated model and material files.
It's probably best to copy the `raytracer` executable into that directory.
The example renders are also provided, but in `.png` instead of `.ppm` format, as discussed in my email. 

The first, `driver02.txt`, renders a neat image that probably came from either a D&D campaign or some kind of metal album from the 1990s.
It takes about 14 minutes to render on my machine because I'm bad at programming ray/triangle intersections and didn't get time to implement any sort of space-saving data structure thingy.
If this is too much time, I would recommend decreasing the resolution.

The second, `driver03.txt`, renders a rather large number of spheres.
The output image of the program will be just over a hundred megabytes, so... make sure you have disk space, I guess!
This renders at `4000x4000`, so you can decrease the resolution if you'd rather not waste space.
Still, even with the high resolution, it only takes four minutes or so to render on my machine.


## Notes
- This program will chew up 100% of the CPU time as it runs in paralell with enough threads to keep every core busy.
  If the entire machine grinds to a halt while running it on a complex scene, this is probably why.
- On extremely large images (somewhere around the tens of thousand of pixels wide or tall) this code may not work due to memory issues.
- This program assumes that we should use the only material in a .mtlib file for all faces if there is only one material in that file.
  It will otherwise perform colorization correctly, although it will pick a random material for any faces before the first `usemtl` line.