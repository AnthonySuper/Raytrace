# Raytracer

Raytracer is a recursive raytracer I wrote for a school project.
It features parallel ray tracing, recursive ray tracing, reflections, semitransparent materials, and refraction.
It also uses a Bounding Volume Hierarchy in order to trace things faster.
It's not quite as optimized as it theoretically could be, but it's decently fast, and makes good use of CPU cores.

## Gallery

![An example image](custom_drivers/driver01-small.png)
![Another example image](custom_driver/driver00.png)

There's also a [neat animated video](https://www.youtube.com/watch?v=xh-0_TiNCvU) made by tracing frames individually and then stitching them together with a script.

## Building

### Get Dependencies 
You'll need `libpng` in your path.

### Compiling
Unpack the tar.
Then, run this command.

```bash
make
```

You will need a C++ compiler that supports C++14.
Note that this will compile with `-Ofast -march=native -flto`, so the speed of compilation will be rather slow.
Sorry.

If you have troubles, try running `make clean` first.

## Running

Run this command:

```bash
./raytracer DRIVERFILE OUTFILE
```

Where `DRIVERFILE` is a path to a Driver File.
Driver Files are described later on in this README, but an example can be found [here](custom_drivers/driver01.txt).
This code will output how much progress it's made every second or so, as a percentage.
It will render the image into `OUTFILE`.
This program uses the excellent [CImg](http://cimg.eu/) library to output pixel data.
As a result, `OUTFILE` will smartly read the file type from the extension, and save the image with the appropriate type.
I recommend the use of `.png`.

## Features
This raytracer makes us of a *bounding volume hierarchy*, *texture mapping*, and *smooth shading*.

A *bounding volume hierarchy* is an acceleration data structure for raytracing.
It essentially puts all the primitives inside nested boxes, then uses a smart traversal algorithm to test against a minimal number of boxes and primitives.
There's a variety of ways to construct a tree of this sort.
I use a *lazy SAH*, which basically finds the best surface area heuristic (the box with the smallest surface area) out of ten possible boxes, and uses that as a split.
Despite being extremely lazy, this seems to work well in practice, and it drastically reduces the time to render large images (even if the startup cost is a bit higher now).

*Texture Mapping* is the process of applying textures to faces.
I use the excellent [CImg](http://cimg.eu/) library to read the actual texture files.
This requires linking in `libpng`, so make sure you have that available.
I then use the wavefront UV coordinates to apply textures.
This is fairly simple to do, but the code is in `src/face.cpp` if you want a look.

I also do *smooth shading*.
This is the process of using the object's built-in vertex normals to smoothly shade the object based on where a ray hit, and is *much* nicer looking than simply finding the normal of the triangle!
Unfortunately, this also means that objects without proper normals won't actually render, but all the models contained in this example do.
If you would like to use this raytracer to render other things (possibly as part of grading), make sure the objects have valid normal information!

## Example Renderings
All of the example renderings are located in `custom_drivers`.
Copy the `raytracer` executable into that directory create them.

The first custom rendering is generated by `custom_driver/driver01.txt`
Make it with:

```bash
make clean && make -j 16 && cp raytracer custom_drivers && cd custom_drivers && ./raytracer driver00.txt driver00-self-rendered.png
```

It demonstrates refraction by refracting light though several spheres onto a pretty rainbow surface.


The second custom driver, is located in `custom_drivers/driver01.txt`
Make it by running:

```bash
make clean && make -j 16 && cp raytracer custom_drivers && cd custom_drivers && ./raytracer driver01.txt driver01-self-rendered.png
```

This file demonstrates texture mapping, smooth shading, refraction, and all the other features of my raytracer.
It's also *really* large, so watch out for that.
Some image viewers don't like images this big, so you may wish to modify the resolution down to `1600x900` or some similarly smaller resolution before viewing.
Alternatively, resize the image with the `convert` command.


## Notes
- This program will chew up 100% of the CPU time as it runs in paralell with enough threads to keep every core busy.
  If the entire machine grinds to a halt while running it on a complex scene, this is probably why.
- This program assumes that we should use the only material in a .mtlib file for all faces if there is only one material in that file.
  It will otherwise perform colorization correctly, although it will pick a random material for any faces before the first `usemtl` line.
- In *extremely* rare cases, the BVH construction will hit a strange memory error and crash.
  I have no idea why this happens, but it should not occur on the provided driver files.

