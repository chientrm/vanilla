# My C Project

This project is a simple C application that demonstrates the structure of a C project with separate source and header files. It includes multiple programs written in C, including a main program and a Julia set generator using OpenGL.

## Project Structure

```
my-c-project
├── bin
│   ├── my-c-project
│   └── julia-set
├── include
│   ├── stb_image_write.h
│   └── utils.h
├── obj
├── src
│   ├── main.c
│   ├── utils.c
│   └── julia_set.c
├── images
│   └── julia_set.png
├── Makefile
└── README.md
```

## Building the Project

To build the project, you need to have `gcc` and `make` installed on your system. Run the following command to build all targets:

```
make
```

This will compile the source files and generate the binaries in the `bin` directory.

## Running the Programs

### Main Program

To run the main program, use the following command:

```
./bin/my-c-project
```

### Julia Set Generator

To run the Julia set generator, use the following command:

```
./bin/julia-set
```

This will generate a Julia set image and save it as `images/julia_set.png`.

## Cleaning Up

To clean up the build artifacts, run the following command:

```
make clean
```

This will remove the object files, binaries, and the `obj` and `bin` directories.

## Description

- `src/main.c`: Entry point of the application.
- `src/utils.c`: Contains utility functions.
- `src/julia_set.c`: Generates a Julia set using OpenGL and saves it as a PNG image.
- `include/utils.h`: Header file for utility functions.
- `include/stb_image_write.h`: Header file for image writing functions.

## License

This project is licensed under the MIT License.