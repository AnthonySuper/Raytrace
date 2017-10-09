# Raytracer

This is a program which does "raytracing" (in the sense that it throws rays into a scene and gets their depth values).
It then maps these depth values into a nice color.


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
- I talked to Dr. Beveridge in his office hours about a numerical stability issue my code runs into on the test driver #2.
  Weirdly, this issue only pops up on my machine, and not the CS machines!
  I suspect this is due to a compiler difference.
  However, he did want me to mention this in my README, in case there was some problem.
- This program will chew up 100% of the CPU time as it runs in paralell with enough threads to keep every core busy.
  If the entire machine grinds to a halt while running it on a complex scene, this is probably why.
- On extremely large images (somewhere around the tens of thousand of pixels wide or tall) this code may not work.
