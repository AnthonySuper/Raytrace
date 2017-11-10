# Raytracer

This is a program which does "raytracing" (albiet non-recursively).


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

## Notes
- This program will chew up 100% of the CPU time as it runs in paralell with enough threads to keep every core busy.
  If the entire machine grinds to a halt while running it on a complex scene, this is probably why.
- On extremely large images (somewhere around the tens of thousand of pixels wide or tall) this code may not work due to memory issues.
- This program assumes that we should use the only material in a .mtlib file for all faces if there is only one material in that file.
  It will otherwise perform colorization correctly, although it will pick a random material for any faces before the first `usemtl` line.
