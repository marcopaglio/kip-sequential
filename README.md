# Kip-sequential

This is the *sequential* version of **Kernel Image Processing**, which is a convolution-based filtering applied to 2D images using a variable size kernel. Parallelized versions of kip-sequential can be found at:

- [kip-parallel-openMP](https://github.com/marcopaglio/kip-parallel-openMP "Repository of kip-parallel-openMP")
- [kip-parallel-CUDA](TODO "Repository of kip-parallel-CUDA")

## Introduction

Kernel Image Processing is used to enhance, filter, and analyze images. It involves applying a small squared matrix of odd order, known as a **kernel**, to a 2D image to perform transformations, such as blurring, sharpening, embossing, edge detection, and more. This is accomplished by doing a **convolution** between the kernel and an image, that is:

1. The kernel moves across the image pixel by pixel.
2. At each position, it multiplies its values with the corresponding pixel values in the image.
3. The results are summed up and replace the central pixel.

This process is repeated for all pixels, generating a transformed image.

<p align="center">
  <img src="/../assets/2D_Convolution_Animation.gif" alt="Screenshot of the simple BookingApp GUI." title="BookingApp GUI" width="30%"/>
</p>

The general expression of the convolution of an image $I$ with a kernel $K$ is defined as:<br>

$$T(x, y) = \sum_{m=-k}^k \sum_{n=-k}^k K(m, n) \cdot I(x+m, y+n) $$

where $x$ and $y$ are the coordinates of the pixel in the 2D image, $T$ is the transformed image, and every element of $K$ is considered by $-k \leq m \leq k$ and $-k \leq n \leq k$, where $k$ is half the kernel size approximated by default (e.g. for a 3x3 kernel, k = 1).

This can be described algorithmically with the following pseudo-code:
```
 height ← image.height
 width ← image.width
 k ← kernel.size // 2

 Create new_image of size (height, width)
 For y from 0 to height:
     For x from 0 to width:
         sum ← 0
         For i from -k to k:
             For j from -k to k:
                sum ← sum + (kernel[k + i, k + j] * image[y + i, x + j])
         new_image[y, x] ← sum
```

### Edge handling

Kernel convolution requires values from pixels outside of the image boundaries. There are a variety of methods for handling image edges:

- **Constant Padding**: use constant value for pixels outside of image, usually black pixels (zeros values).
  ```
  Example:
                                  [ 0  0  0  0  0 ]
            [ 1  2  3 ]           [ 0  1  2  3  0 ]
    from    [ 4  5  6 ]     to    [ 0  4  5  6  0 ] 
            [ 7  8  9 ]           [ 0  7  8  9  0 ] 
                                  [ 0  0  0  0  0 ]
  ```

- **Extend**: the border pixels are duplicated to extend the image.
  ```
  Example:
                                  [ 1  1  2  3  3 ]
            [ 1  2  3 ]           [ 1  1  2  3  3 ]
    from    [ 4  5  6 ]     to    [ 4  4  5  6  6 ] 
            [ 7  8  9 ]           [ 7  7  8  9  9 ] 
                                  [ 7  7  8  9  9 ]
  ```
- **Wrap**: pixels are taken from the opposite side of the image.
  ```
  Example:
                                  [ 9  7  8  9  7 ]
            [ 1  2  3 ]           [ 3  1  2  3  1 ]
    from    [ 4  5  6 ]     to    [ 6  4  5  6  4 ]
            [ 7  8  9 ]           [ 9  7  8  9  7 ]
                                  [ 3  1  2  3  1 ]
  ```
- **Mirror**: the image is extended by mirroring the border pixels.
  ```
  Example:
                                  [ 5  4  5  6  5 ]
            [ 1  2  3 ]           [ 2  1  2  3  2 ]
    from    [ 4  5  6 ]     to    [ 5  4  5  6  5 ]
            [ 7  8  9 ]           [ 8  7  8  9  8 ]
                                  [ 5  4  5  6  5 ]
  ```
- **Crop**: any pixel which would require values from beyond the edge is skipped. This method reduces the output image size (except with a kernel of order 1).

### Kernel types

Depending on the element values, a kernel can cause a wide range of effects or extract different features, such as:

- **Identity**: the output image is not transformed.
  ```
  Example 3x3:                          Example 5x5:      
                                                        [ 0  0  0  0  0 ]
                  [ 0  0  0 ]                           [ 0  0  0  0  0 ]
                  [ 0  1  0 ]                           [ 0  0  1  0  0 ]
                  [ 0  0  0 ]                           [ 0  0  0  0  0 ]
                                                        [ 0  0  0  0  0 ]
  ```
- **Box Blur**: a simple averaging filter that replaces each pixel with the mean of its neighbors.
  ```
  Example 3x3:                          Example 5x5:    
                                                        [ 1  1  1  1  1 ]
                  [ 1  1  1 ]                           [ 1  1  1  1  1 ]
              1/9 [ 1  1  1 ]                      1/25 [ 1  1  1  1  1 ]
                  [ 1  1  1 ]                           [ 1  1  1  1  1 ]
                                                        [ 1  1  1  1  1 ]
  ```
- **Edge Detection**: highlight changes in intensity, useful for detecting shapes and boundaries.
  ```
  Example 3x3:                          Example 5x5:    
                                                        [ -1  -1  -1  -1  -1 ]
                  [ -1  -1  -1 ]                        [ -1  -2  -2  -2  -1 ]
                  [ -1   8  -1 ]                        [ -1  -2  32  -2  -1 ]
                  [ -1  -1  -1 ]                        [ -1  -2  -2  -2  -1 ]
                                                        [ -1  -1  -1  -1  -1 ]
  ```
- **Sharpen**: enhance edges and details by amplifying differences.
  ```
  Example 3x3:                          Example 5x5:    
                                                        [  0   0  -1   0   0 ]
                  [  0  -1   0 ]                        [  0  -1  -2  -1   0 ]
                  [ -1   5  -1 ]                        [ -1  -2  17  -2  -1 ]
                  [  0  -1   0 ]                        [  0  -1  -2  -1   0 ]
                                                        [  0   0  -1   0   0 ]

  ```

## Implementation Details

Kip-sequential is written in **C++** and uses **CMake** as build automation tool. The structure of the code is described by the following class diagram:

<p align="center">
  <img src="/../assets/UML_classDiagram.jpg" alt="UML Class Diagram of Kernel Image Processing." title="Class Diagram" width="70%"/>
</p>

- entities (**Pixel**, **Image** and **Kernel**) are implemented as read-only: no setter or other modifier are defined, so that image processing functions must instantiate new objects instead of modifying the existing ones.
- pixels are stored as a matrix, i.e. `vector<vector<Pixel>>`, in order to access the elements clearly; unfortunately, this way incurs considerable overhead because of the *Standard Template Library* (STL). Alternative versions of this data structure are proposed in the [pixel_vector](/../pixel_vector) branch, in which pixels are stored as a single vector, i.e. `vector<Pixel>`, and [pixel_SoA](/../pixel_SoA) branch, which red, green and blue values are stored in indipendent vectors, i.e. `vector<uint_8>`.
- kernels differ only in the way they are constructed; for this reason, **KernelFactory** has a static method for each type that builds kernel values based on its order. In particular, only *box blur* and *edge detection* kernels described in the Section [Kernel types](#kernel-types) are used.
- the processing core (**ImageProcessing**) collects the functions that modify images. In particular:
  * `extendEdge` generates a new image with the edges extended by `padding` pixels on each side using the *extend* method described in the Section [Edge handling](#edge-handling);
  * `convolution` creates a transformed image by applying convolution of the input image with the input kernel, as described in the [Introduction](#introduction). It consists of four nested loops:
    ```
    for (unsigned int y = 0; y < outputHeight; y++)
        for (unsigned int x = 0; x < outputWidth; x++)
            for (unsigned int j = 0; j < order; j++)
                for (unsigned int i = 0; i < order; i++)
    ```
    where `outputWidth` and `outputHeight` are the dimension of the transformed image, and `order` is the dimension of the kernel. Before creating a new pixel, its values are conformed from 0 to 255 even if the transformation had given them an out-of-range value.<br><br>
  
  > :bulb: **Tip**: Actually, edge handling responsability lies with the programmer which should call `extendEdge` with the right padding, i.e. the half kernel order, before `convolution`. If `extendEdge` is not call, `convolution` works as well, but the transformed image has sizes cropped with respect to the input one. Same thing if `extendEdge` is called with `0` as padding.
  > 
  > An alternative version is presented in the [edgeHandler_strategy](/../edgeHandler_strategy) branch, in which edge handling is injected into the **ImageProcessing** class and used appropriately just before the image convolution. However, it introduces some overhead and forces to create the extended image each time, rather than once.

- [**stb**](https://github.com/nothings/stb "GitHub repository of stb") is a collection of single-file header-file libraries for C/C++ used to:
  * retrieve data (i.e. width, height, channels and pixel values) from an image specified by the path, through its `stbi_load` function, which also converts them in RGB images;
  * save the transformed image into a new JPG image through its `stbi_write_jpg` function.
  
  In both cases, it stores the RGB pixel sequentially as an array of `unsigned char`, so proper convertion from/to the format used in the code is required. To use *stb*, you must include its two header files (`stb_image.h`, `stb_image_write.h`) in your project and then use the following definitions and inclusions in the code in which it is used:
  ```
  #define STB_IMAGE_IMPLEMENTATION
  #define STB_IMAGE_WRITE_IMPLEMENTATION
  #include "stb_image.h"
  #include "stb_image_write.h"
  ```

  > :warning: **Warning**: As written in [stb project](https://github.com/nothings/stb/blob/master/README.md "README file of stb GitHub repository"), some security-relevant bugs are discussed in public in Github, so it is strongly recommended to do not use the stb libraries.

### Main Program

The goal of the project is to measure the sequential execution time of Kernel Image Processing and then to compare it with its parallel versions. The comparison works well only if the **wall-clock time** is used, since the *processor time* is about the same in both sequential and parallel versions.<br>

In C++, this can be done through the standard chrono library with its classes `high_resolution_clock` and `steady_clock`. The first one is more precise because uses the smallest tick period provided by the implementation, but to have results more consistent and reproducible it requires the steadiness of the clock, i.e. the time between ticks should be always constant even in case of some external clock adjustment. The *C++ toolchain* (MSVC, MinGW, etc) choosen for the platform doesn't always work stably, so, as an alternative, the `steady_clock` forces to use monotonic clock but it might be less sensitive than `high_resolution_clock`. To use the best available one, in the main program a simple check is done through the `std::chrono::high_resolution_clock::is_steady` function.<br>

In particular, on Windows I experimented that MSVC (*v.17.0*, with both MSVC and Clang compilers) works with steadiness, therefore `high_resolution_clock` is usable; while MinGW (*v.11.0*) doesn't provide steadiness, hence `steady_clock` is better.

Because the aim of the project, the chrono is started just before the call to the `convolution` method, and it is ended as soon as it finished. Each image is processed multiple times (e.g. 3) to reduce external system overheads and obtain a more reliable time measurement. Other uninfluent execution parts of the main code, such as the loading/storing of the image and the kernel construction, are not time recorded. <br>

Kernel types are of type `box blur` and `edge detection`. For each of them, different orders are used: `7`, `13`, `19`, `25`.

Tests are done on very large images of random subjects in order to obtain more advantages from the parallel execution; in fact, parallelization works better if data are sufficiently big. Choosen images are called:
- `4K-1`, `4K-2`, `4K-3` which have dimension 4000x2000 pixels;
- `5K-1`, `5K-2`, `5K-3` which have dimension 5000x3000 pixels;
- `6K-1`, `6K-2`, `6K-3` which have dimension 6000x4000 pixels;
- `7K-1`, `7K-2`, `7K-3` which have dimension 7000x5000 pixels.

They can be found in the [input](./src/imgs/input) folder.<br>

Time measuraments of filtering with the different kernel type on the different images are summerized in the following table. Used hardware is:
- [Intel Core i7-12650H](https://www.intel.com/content/www/us/en/products/sku/226066/intel-core-i712650h-processor-24m-cache-up-to-4-70-ghz/specifications.html "Specification page for Intel Core i7-12650H"): 2.30 GHz up to 4.70 GHz, 10 cores (6 Performance, 4 Efficient), 16 threads, 24 MB of cache L3;
- 16 GB of RAM DDR5 (4800 MHz)
- Windows 11 Home

<table>
  <thead>
    <tr>
      <th colspan="3" rowspan="3">Execution Time</th>
      <th colspan="12">Image Dimension</th>
    </tr>
    <tr>
      <th colspan="3">4K</th>
      <th colspan="3">5K</th>
      <th colspan="3">6K</th>
      <th colspan="3">7K</th>
    </tr>
    <tr>
      <th>1</th>
      <th>2</th>
      <th>3</th>
      <th>1</th>
      <th>2</th>
      <th>3</th>
      <th>1</th>
      <th>2</th>
      <th>3</th>
      <th>1</th>
      <th>2</th>
      <th>3</th>
    </tr>
  </thead>
  <tbody>
    <tr>
      <td rowspan="8"><strong>Kernel Dimension</strong></td>
      <td rowspan="4"><strong>Box Blurring</strong></td>
      <td>7</td>
      <td></td>
      <td></td>
      <td></td>
      <td></td>
      <td></td>
      <td></td>
      <td></td>
      <td></td>
      <td></td>
      <td></td>
      <td></td>
      <td></td>
    </tr>
    <tr>
      <td>13</td>
      <td></td>
      <td></td>
      <td></td>
      <td></td>
      <td></td>
      <td></td>
      <td></td>
      <td></td>
      <td></td>
      <td></td>
      <td></td>
      <td></td>
    </tr>
    <tr>
      <td>19</td>
      <td></td>
      <td></td>
      <td></td>
      <td></td>
      <td></td>
      <td></td>
      <td></td>
      <td></td>
      <td></td>
      <td></td>
      <td></td>
      <td></td>
    </tr>
    <tr>
      <td>25</td>
      <td></td>
      <td></td>
      <td></td>
      <td></td>
      <td></td>
      <td></td>
      <td></td>
      <td></td>
      <td></td>
      <td></td>
      <td></td>
      <td></td>
    </tr>
    <tr>
      <td rowspan="4"><strong>Edge Detection</strong></td>
      <td>7</td>
      <td></td>
      <td></td>
      <td></td>
      <td></td>
      <td></td>
      <td></td>
      <td></td>
      <td></td>
      <td></td>
      <td></td>
      <td></td>
      <td></td>
    </tr>
    <tr>
      <td>13</td>
      <td></td>
      <td></td>
      <td></td>
      <td></td>
      <td></td>
      <td></td>
      <td></td>
      <td></td>
      <td></td>
      <td></td>
      <td></td>
      <td></td>
    </tr>
    <tr>
      <td>19</td>
      <td></td>
      <td></td>
      <td></td>
      <td></td>
      <td></td>
      <td></td>
      <td></td>
      <td></td>
      <td></td>
      <td></td>
      <td></td>
      <td></td>
    </tr>
    <tr>
      <td>25</td>
      <td></td>
      <td></td>
      <td></td>
      <td></td>
      <td></td>
      <td></td>
      <td></td>
      <td></td>
      <td></td>
      <td></td>
      <td></td>
      <td></td>
    </tr>
  </tbody>
</table>

## Unit Test

To ensure sequential and parallel versions work, the application code is supported by unit tests.<br>

Tests are written through the [GoogleTest](https://github.com/google/googletest "GitHub repository of GoogleTest") framework, configured in the `CMakeLists.txt` located in the [tests](./tests) directory.<br>

Entities' tests (**PixelTest**, **ImageTest**, and **KernelTest**) are quite simple and they just verify the constructor or default constructor behaviour.

> :pencil: **Note**: Assertions uses `EXPECT_EQ` if its failure doesn't affect subsequent tests, or `ASSERT_EQ` if its truthfulness is necessary for the next ones.

Tests for the kernel factory class (**KernelFactoryTest**) aims to checks the validity of kernel attributes' value, in particular of the oddity of the dimsnion of the kernel; then to verify the construction of the kernel respects its type with different orders. Because tests differ only in values, parameterized tests are here used. These checks are done for each type of kernel created, i.e. for each method of the class.<br>

Testing a third-party library is a bad practice in unit testing, so for testing the STB functions a wrapper class has been istantiated, i.e. facade design pattern is applied. This is the reason why **STBImageReader** inherits from the abstract class **ImageReader**: in this way, even if the behaviour of STB changes, the main code will not change, but only the wrapper class. Anyway, tests for the loading mathod uses a simple and well-known JPG image to check if expected values are retrived from the image through the library; tests for the saving method just checks if an JPG image file is created after the library is called (without checking if values are correct, because this should rely on a library itself to read the content). For both methods, tests checks also that an exception is thrown if the path is not correct.<br>

The most important tests for the project are for the image processing algorithms. For the image convolution, pixel values of the transformed image should be in accord to the image and kernel input, manually calcolated through the formula defined in the [introduction](#introduction); some tests forces the transformed image to have out-of-range values for pixels, so that they can check if in-range convertion works. For the edge extention, the new dimensions of the extented image are checked, as well as both old (in the middle) and new (in the edges) pixel values, in accord to the input image and padding. In both functions, tests check that the returned image is a new one.

## ASAN

In the main `CmakeLists.txt` few lines of configuration code are inserted to enable the [Google AddressSanitizer](https://github.com/google/sanitizers/wiki/addresssanitizer "GitHub Repository of ASan") tool to check if there are no memory issues:
```
if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC" AND USE_ASAN)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreadedDLL")
    add_compile_options(/fsanitize=address /Zi)
endif()
```
Currently, on Windows, this tool works well only with the MSVC compiler. Even if this project is built with LLVM/Clang, switching to MSVC does not need accommodations other than changing the compiler itself.<br>

ASan instruments the code and generates an executable that substitute allocation/deallocation methods with its equivalent methods. In this way, memory issues are recognized and errors are print when the executable is executed. By the way, the overhead introduced in the program grows with the code complexity, and the execution time increases. Because this overhead is not always needed, the tool is turned on only when the Cmake option `-DUSE_ASAN=ON` is set in the used profile (e.g. Debug, Release).


## Profiler

TODO
