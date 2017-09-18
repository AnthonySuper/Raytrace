# Modeltoworld

Model2world is a simple program that performs model-to-world translations on .obj files.
It is intended to be the basis for an eventual raytracer.


## Compilation
You can compile this program with `make`.
Be sure to run `make` with **no arguments** as the makefile also builds unit tests, the code for which is not included in distributions.
This process make take some time due to the level of optimization enabled (which is fairly high).

It should output an executable called `modeltoworld`.

Note: the makefile used is identical to one I used in another class.

## Usage

Run `./modeltoworld DRIVER` where `DRIVER` is a Driverfile in the specified format.
All models to load should be in the current working directory of modeltoworld.
The program will then create a new directory equal to the basename of the driverfile (the filename minus the extension) and output transformed models in this directory with the given naming scheme.
This is very fast for all test files, but may be horrendously slow for extremely large object files.

## Errata
This program does not currently support MS Windows.
It will (maybe) do so in the future.


