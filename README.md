# kip-sequential

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

## Implementation

The code's structure is fully described by the following class diagram:

<p align="center">
  <img src="/../assets/UML_classDiagram.jpg" alt="UML Class Diagram of Kernel Image Processing." title="Class Diagram" width="70%"/>
</p>

The implementation in **C++** uses **CMake** and is intentionally kept simple:

- the three entities (**Pixel**, **Image** and **Kernel**) are read only: they have no setter or other modifier methods, so how they remain as when they are built with the constructor. As a consequence, image processing's functions have to instantiate new objects instead of modififying the existing ones.
- pixels in the image are stored as vectors of vectors, i.e. `vector<vector<Pixel>>`, in order to access the elements of the image with a matrix. This involves in lots of overhead because the use of STL, but code is clearer. Alternative versions of this data structure are proposed in [pixel_vector](/../pixel_vector) and [pixel_SoA](/../pixel_SoA) branches, where pixels are stored as a unique vector, i.e. `vector<Pixel>`, and three simple vectors of red, green and blue values, i.e. `vector<uint_8>`, respectively.
- there is no need to have multiple kernel type classes because they all behave the same way, but the way they are constructed depends on their type. For this reason, **KernelFactory** has a static method for each kernel type which builds the kernel with the appropriate values based on its order.
- to read pixel values from images (JPG, PNG, etc) and to save the transformed image into a JPG (or similarly in other formats) images, an external library is used: [**stb**](https://github.com/nothings/stb "GitHub repository of stb") . It just needs to include in the project its two header files (`stb_image.h`, `stb_image_write.h`), and then to use the following definitions and inclusions in the code where it is used:
  ```
  #define STB_IMAGE_IMPLEMENTATION
  #define STB_IMAGE_WRITE_IMPLEMENTATION
  #include "stb_image.h"
  #include "stb_image_write.h"
  ```
  The library uses its `stbi_load` function to recover data (width, height, channels and pixel values) from a path specified image, and `stbi_write_jpg` to store a new JPEG image. In both cases, pixel values are stored as an array of chars, i.e. `unsigned char*` where RGB values of a pixel are stored sequentially, so values have to be converted properly from the format used in the code with that used by stb. If the loading fails, `NULL` is returned; if the storing fails `stbi_write_jpg` returns `0`.
- **ImageProcessing** gathers together functions that modifies Images. In particular:
  * `extendEdge` creates a new image similar to the input one but with edges extended by `padding` pixels for each side with the *extend* method described in [edge handling](#edge-handling);
  * `convolution` creates a transformed image starting from the input one by applying [image convolution](#introduction) through the input kernel. It consists of four annidated cycles:
    ```
    for (unsigned int y = 0; y < outputHeight; y++)
        for (unsigned int x = 0; x < outputWidth; x++)
            for (unsigned int j = 0; j < order; j++)
                for (unsigned int i = 0; i < order; i++)
    ```
    where `outputWidth` and `outputHeight` are the dimension of the transformed image, and `order` is the dimension of the kernel. Before creating a new pixel, its values are conformed from 0 to 255 even if the transformation had given them an out-of-range value.<br><br>
  
  > :bulb: **Tip**: Actually, edge handling responsability lies with the programmer which should call `extendEdge` with the right side dimension, i.e. the half kernel order, before `convolution`. If `extendEdge` is not call, `convolution` works as well, but the transformed image has sizes cropped with respect to the input one. Same thing if `extendEdge` is called with `0` as padding.
  

## Unit Test

To ensure sequential and parallel versions work, the application code is supported by unit tests.<br>

Tests are written through the [GoogleTest](https://github.com/google/googletest "GitHub repository of GoogleTest") framework, configured in the `CMakeLists.txt` located in the [tests](./tests) directory.<br>

Entities' tests (**PixelTest**, **ImageTest**, and **KernelTest**) are quite simple and they just verify the constructor or default constructor behaviour.

> :pencil: **Note**: Assertions uses `EXPECT_EQ` if its failure doesn't affect subsequent tests, or `ASSERT_EQ` if its truthfulness is necessary for the next ones.

Tests for the kernel factory class (**KernelFactoryTest**) aims to checks the validity of kernel attributes' value, in particular of the oddity of the dimsnion of the kernel; then to verify the construction of the kernel respects its type with different orders. Because tests differ only in values, parameterized tests are here used. These checks are done for each type of kernel created, i.e. for each method of the class.<br>

Testing a third-party library is a bad practice in unit testing, so for testing the STB functions a wrapper class has been istantiated, i.e. facade design pattern is applied. This is the reason why **STBImageReader** inherits from the abstract class **ImageReader**: in this way, even if the behaviour of STB changes, the main code will not change, but only the wrapper class. Anyway, tests for the loading mathod uses a simple and well-known JPG image to check if expected values are retrived from the image through the library; tests for the saving method just checks if an JPG image file is created after the library is called (without checking if values are correct, because this should rely on a library itself to read the content). For both methods, tests checks also that an exception is thrown if the path is not correct.<br>

The most important tests for the project are for the image processing algorithms. For the image convolution, pixel values of the transformed image should be in accord to the image and kernel input, manually calcolated through the formula defined in the [introduction](#introduction); some tests forces the transformed image to have out-of-range values for pixels, so that they can check if in-range convertion works. For the edge extention, the new dimensions of the extented image are checked, as well as both old (in the middle) and new (in the edges) pixel values, in accord to the input image and padding. In both functions, tests check that the returned image is a new one.

## Main Program

Aim of this project is to measure the execution time of sequential execution of kernel image filtering, compared with its parallel versions. The comparison works well only if the **wall-clock time** is used because it measure the elapsed real-time instead of processor time which is the same (more or less) amount sequential and parallel versions. In C++, this can be done through the standard chrono library with its classes `high_resolution_clock` and `steady_clock`. The last one is more reliable but requires the steadyness of the clock, i.e. the time between ticks should be always constant even in case of some external clock adjustment. Not always the system (TODO: di chi è la colpa??) works in this way, so as alternative the first one is used and uses the smallest tick period provided by the implementation. In order to use, the best available one, in the main program a simple check (`is_steady`) is done to decide which to use. Because chrono functions have different names, wrapper classes are used to make the main code uniform.

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
      <th></th>
      <th></th>
      <th>4K</th>
      <th>5K</th>
      <th>6K</th>
      <th>7K</th>
    </tr>
  </thead>
  <tbody>
    <tr>
      <td></td>
      <td><strong>Kernel Dimension</strong></td>
      <td colspan="4"><strong>Execution Time</strong></td>
    </tr>
    <tr>
      <td rowspan="4"><strong>Box Blurring</strong></td>
      <td>7</td>
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
    </tr>
    <tr>
      <td>19</td>
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
    </tr>
    <tr>
      <td rowspan="4"><strong>Edge Detection</strong></td>
      <td>7</td>
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
    </tr>
    <tr>
      <td>19</td>
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
    </tr>
  </tbody>
</table>

## ASAN

In the main `CmakeLists.txt` few lines of configuration code are inserted to enable the [Google AddressSanitizer](https://github.com/google/sanitizers/wiki/addresssanitizer "GitHub Repository of ASan") tool to check if there are no memory issues:
```
if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC" AND USE_ASAN)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreadedDLL")
    add_compile_options(/fsanitize=address /Zi)
endif()
```
Currently, on Windows, this tool works well only with the MSVC compiler. Even if this project is built with LLVM/Clang, switching to MSVC does not need accommodations other than changing the compiler itself.<br>

ASan instruments the code and generates an executable that substitute allocation/deallocation methods with its equivalent methods. In this way, memory issues are recognized and errors are print when the executable is executed. By the way, the overhead introduced in the program grows with the code complexity, and the execution time increases. Because this overhead is not always needed, the tool is turned on only when the Cmake option `USE_ASAN=ON` is set in the used profile (e.g. Debug, Release).
