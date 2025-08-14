# Kip-sequential

This is the *sequential* version of **Kernel Image Processing**, which is a convolution-based filtering applied to 2D images using a variable size kernel. Parallelized versions of kip-sequential can be found at:

- [kip-parallel-OpenMP](https://github.com/marcopaglio/kip-parallel-openMP "Repository of kip-parallel-OpenMP")
- [kip-parallel-CUDA](TODO "Repository of kip-parallel-CUDA")

## Table of Contents

- [Introduction](#introduction)
  + [Edge Handling](#edge-handling)
  + [Kernel Types](#kernel-types)
- [Implementation Details](#implementation-details)
- [Experimentations](#experimentations)
  + [Experiment Variables](#experiment-variables)
  + [Timing](#timing)
  + [Experimental Results](#experimental-results)
  + [Hardware Details](#hardware-details)
- [Further Checks](#further-checks)
  + [Unit Testing](#unit-testing)
  + [Address Sanitization](#address-sanitization)
  + [Profiling](#profiling)

## Introduction

Kernel Image Processing is used to enhance, filter, and analyze images. It involves applying a small square matrix of odd order, known as a **kernel**, to a 2D image to perform transformations, such as blurring, sharpening, embossing, edge detection, and more. This is accomplished by doing a **convolution** between the kernel and an image, that is:

1. The kernel moves across the image pixel by pixel.
2. At each position, it multiplies its values with the corresponding pixel values in the image.
3. The results are summed up and replace the central pixel.

This process is repeated for all pixels, generating a transformed image.

<p align="center">
  <img src="/../assets/2D_Convolution_Animation.gif" alt="Animation of 2D convolution." title="2D Convolution" width="30%"/>
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

### Edge Handling

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

### Kernel Types

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
- kernels differ only in the way they are constructed; for this reason, **KernelFactory** has a static method for each type that builds kernel values based on its order. In particular, only *box blur* and *edge detection* kernels described in the Section [Kernel Types](#kernel-types) are used.
- the processing core (**ImageProcessing**) collects the functions that modify images:
  * `extendEdge` generates a new image with the edges extended by `padding` pixels on each side using the *extend* method described in the Section [Edge Handling](#edge-handling).
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
  * retrieve data (i.e. width, height, channels and pixel values) from an image specified by the path, through its `stbi_load` function, which also converts them in RGB images.
  * save the transformed image into a new JPG image through its `stbi_write_jpg` function.
  
  In both cases, it stores RGB pixels sequentially as an array of `unsigned char`, so proper convertion from/to the format used in the code is required. To use *stb*, you must include its two header files (`stb_image.h`, `stb_image_write.h`) in your project and then use the following definitions and inclusions in the code in which it is used:
  ```
  #define STB_IMAGE_IMPLEMENTATION
  #define STB_IMAGE_WRITE_IMPLEMENTATION
  #include "stb_image.h"
  #include "stb_image_write.h"
  ```

  > :warning: **Warning**: As written in [stb project](https://github.com/nothings/stb/blob/master/README.md "README file of stb GitHub repository"), some security-relevant bugs are discussed in public in Github, so it is strongly recommended to do not use the stb libraries.

## Experimentations

The goal of the project is to measure the sequential execution time of Kernel Image Processing and then to compare it with its parallel versions.

### Experiment Variables

#### Images

Tests are run on very large images to get more benefits from parallel execution; in fact, parallelization works better if the data is sufficiently large. The selected images can be found in the [input](imgs/input) folder and are called according to their *quality*:
- `4K` if the size is 4000x2000 pixels.
- `5K` if the size is 5000x3000 pixels.
- `6K` if the size is 6000x4000 pixels.
- `7K` if the size is 7000x5000 pixels.

For each image size, 3 different images are used in order to evaluate the performance on different and random subjects.

#### Kernels

For each kernel type (i.e. `box blur` and `edge detection`) multiple *order* values are used: `7`, `13`, `19`, `25`. These affect the number of pixels that must be processed to create each new pixel in the transformed image, so they are more decisive than the image size in terms of execution time.

### Timing

The comparison works well only if the **wall-clock time** is used, since the *processor time* is about the same in both sequential and parallel versions.<br>

In C++ this can be done through the standard chrono library with its classes `high_resolution_clock` and `steady_clock`. The first one is more precise because uses the smallest tick period provided by the implementation, but to have results more consistent and reproducible it requires the steadiness of the clock, i.e. the time between ticks should be always constant even in case of some external clock adjustment. The *C++ toolchain* (MSVC, MinGW, etc) choosen for the platform doesn't always work stably, so, as an alternative, the `steady_clock` forces to use monotonic clock but it might be less sensitive than `high_resolution_clock`. To use the best available one, in the main program a simple check is done through the `std::chrono::high_resolution_clock::is_steady` function.<br>

> :pencil: **Note**: On Windows I experimented that MSVC (*v.14.4x* under *Visual Studio 2022 v17.0*, with both MSVC and Clang compilers) works with steadiness, therefore `high_resolution_clock` is usable; while MinGW (*v.11.0 w64*) doesn't guarantee steadiness, hence `steady_clock` is better.<br>

#### What to Measure

Given the aim of the project, the timer starts just before the `convolution` method call and ends as soon as the processing finishes. Other irrelevant parts of the main code, such as loading/storing images and kernel construction, are not time-measured.

#### Reduce Overhead

External or background system processes can influence experiments. To get a more reliable time measurement, two strategies are used:
- Each image is processed `3` times, and the average time is taken. 
- After processing each image with all kernel types of the same size, the execution is paused (using the *sleep* command) so that other processes can be carried on and thus affect time measurements less. The idle time is `imageQuality + order / 2` seconds, where `imageQuality` is the high definition number of the image (e.g. it counts 4 seconds for `4K` images).

### Experimental Results

The following tables summarizes the temporal measurements of convolutions on different images with different kernels, measured in both debug and release mode.

<table>
  <thead>
    <tr>
      <th colspan="3" rowspan="3">Execution Time<br>(Debug mode)</th>
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
      <td rowspan="9"><strong>Kernel Dimension</strong></td>
      <td rowspan="4"><strong>Box Blurring</strong></td>
      <td><strong>7</strong></td>
      <td>3.82795</td>
      <td>3.64721</td>
      <td>3.67625</td>
      <td>14.3222</td>
      <td>6.8883</td>
      <td>16.8814</td>
      <td>11.0175</td>
      <td>16.5867</td>
      <td>23.2189</td>
      <td>16.0264</td>
      <td>16.5054</td>
      <td>16.4069</td>
    </tr>
    <tr>
      <td><strong>13</strong></td>
      <td>12.1591</td>
      <td>11.7018</td>
      <td>11.8204</td>
      <td>49.7128</td>
      <td>48.2678</td>
      <td>56.4534</td>
      <td>36.1269</td>
      <td>78.8835</td>
      <td>61.2037</td>
      <td>51.5812</td>
      <td>51.8145</td>
      <td>51.5229</td>
    </tr>
    <tr>
      <td><strong>19</strong></td>
      <td>24.3588</td>
      <td>24.6647</td>
      <td>36.0659</td>
      <td>102.098</td>
      <td>103.386</td>
      <td>108.489</td>
      <td>75.0609</td>
      <td>166.485</td>
      <td>72.3321</td>
      <td>106.96</td>
      <td>107.461</td>
      <td>107.23</td>
    </tr>
    <tr>
      <td><strong>25</strong></td>
      <td>42.1691</td>
      <td>42.3499</td>
      <td>92.7708</td>
      <td>124.779</td>
      <td>155.358</td>
      <td>190.026</td>
      <td>192.884</td>
      <td>289.44</td>
      <td>125.872</td>
      <td>183.449</td>
      <td>183.899</td>
      <td>183.78</td>
    </tr> 
    <tr>
      <td colspan="14"></td>
    </tr>
    <tr>
      <td rowspan="4"><strong>Edge Detection</strong></td>
      <td><strong>7</strong></td>
      <td>3.90481</td>
      <td>3.73811</td>
      <td>3.71947</td>
      <td>14.2113</td>
      <td>7.16701</td>
      <td>15.4881</td>
      <td>11.3286</td>
      <td>14.9823</td>
      <td>22.4919</td>
      <td>16.3514</td>
      <td>16.5471</td>
      <td>16.6616</td>
    </tr>
    <tr>
      <td><strong>13</strong></td>
      <td>12.0007</td>
      <td>11.7334</td>
      <td>11.9763</td>
      <td>47.0423</td>
      <td>49.6171</td>
      <td>52.1098</td>
      <td>35.9861</td>
      <td>77.8714</td>
      <td>34.3366</td>
      <td>51.6724</td>
      <td>51.6228</td>
      <td>51.7338</td>
    </tr>
    <tr>
      <td><strong>19</strong></td>
      <td>24.3209</td>
      <td>24.6179</td>
      <td>54.0111</td>
      <td>105.43</td>
      <td>102.977</td>
      <td>113.657</td>
      <td>74.3153</td>
      <td>151.77</td>
      <td>72.5981</td>
      <td>106.747</td>
      <td>106.773</td>
      <td>106.392</td>
    </tr>
    <tr>
      <td><strong>25</strong></td>
      <td>41.8965</td>
      <td>42.2515</td>
      <td>92.7604</td>
      <td>77.3546</td>
      <td>180.539</td>
      <td>118.398</td>
      <td>292.527</td>
      <td>285.405</td>
      <td>124.555</td>
      <td>181.092</td>
      <td>182.063</td>
      <td>181.646</td>
    </tr>
  </tbody>
</table>

<table>
  <thead>
    <tr>
      <th colspan="3" rowspan="3">Execution Time<br>(Release mode)</th>
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
      <td rowspan="9"><strong>Kernel Dimension</strong></td>
      <td rowspan="4"><strong>Box Blurring</strong></td>
      <td><strong>7</strong></td>
      <td>1.63491</td>
      <td>1.60292</td>
      <td>1.59928</td>
      <td>3.09233</td>
      <td>2.99461</td>
      <td>2.9685</td>
      <td>4.78064</td>
      <td>4.7717</td>
      <td>4.74233</td>
      <td>6.99629</td>
      <td>6.86251</td>
      <td>6.9645</td>
    </tr>
    <tr>
      <td><strong>13</strong></td>
      <td>4.21743</td>
      <td>4.21682</td>
      <td>4.2429</td>
      <td>7.90956</td>
      <td>7.89317</td>
      <td>7.86825</td>
      <td>12.6152</td>
      <td>12.62</td>
      <td>12.6199</td>
      <td>18.4308</td>
      <td>18.2844</td>
      <td>18.4075</td>
    </tr>
    <tr>
      <td><strong>19</strong></td>
      <td>7.93542</td>
      <td>7.91668</td>
      <td>7.994</td>
      <td>14.8512</td>
      <td>14.8379</td>
      <td>14.8091</td>
      <td>23.7447</td>
      <td>23.713</td>
      <td>23.7582</td>
      <td>34.5979</td>
      <td>34.4332</td>
      <td>34.4969</td>
    </tr>
    <tr>
      <td><strong>25</strong></td>
      <td>12.8117</td>
      <td>12.7387</td>
      <td>12.9757</td>
      <td>23.9107</td>
      <td>23.8816</td>
      <td>23.8889</td>
      <td>38.2216</td>
      <td>38.1825</td>
      <td>38.2566</td>
      <td>55.7258</td>
      <td>55.64</td>
      <td>55.7154</td>
    </tr>
    <tr>
      <td colspan="14"></td>
    </tr>
    <tr>
      <td rowspan="4"><strong>Edge Detection</strong></td>
      <td><strong>7</strong></td>
      <td>1.43027</td>
      <td>1.43156</td>
      <td>1.43082</td>
      <td>2.77367</td>
      <td>2.67681</td>
      <td>2.68797</td>
      <td>4.31606</td>
      <td>4.28377</td>
      <td>4.30037</td>
      <td>6.20926</td>
      <td>6.25295</td>
      <td>6.29526</td>
    </tr>
    <tr>
      <td><strong>13</strong></td>
      <td>3.83239</td>
      <td>3.87818</td>
      <td>3.86391</td>
      <td>7.19051</td>
      <td>7.198</td>
      <td>7.15164</td>
      <td>11.5072</td>
      <td>11.5126</td>
      <td>11.5107</td>
      <td>16.8447</td>
      <td>16.8036</td>
      <td>16.8442</td>
    </tr>
    <tr>
      <td><strong>19</strong></td>
      <td>7.36892</td>
      <td>7.33627</td>
      <td>7.37341</td>
      <td>13.7563</td>
      <td>13.7402</td>
      <td>13.7615</td>
      <td>22.0471</td>
      <td>22.0379</td>
      <td>22.0135</td>
      <td>32.1651</td>
      <td>32.1458</td>
      <td>32.096</td>
    </tr>
    <tr>
      <td><strong>25</strong></td>
      <td>12.0017</td>
      <td>12.0023</td>
      <td>12.345</td>
      <td>22.436</td>
      <td>22.4468</td>
      <td>22.4775</td>
      <td>36.0791</td>
      <td>35.9924</td>
      <td>35.9503</td>
      <td>52.6238</td>
      <td>52.6029</td>
      <td>52.4322</td>
    </tr>
  </tbody>
</table>

#### Save to File

This project records lots of time measurements (96 experiments, i.e. 3 image types x 4 image sizes x 2 kernel types x 4 kernel sizes), therefore saving data in a textual file is desirable. CVS (*Comma-Separated Values*) files, which can be used to generate diagrams programmatically, can be found in the [data](data) folder. In particular, for each experiment the following fields are recorded:
- Input image name
- Input image dimensions
- Kernel type
- Kernel size
- Number of repetitions
- Total convolution time (in seconds)
- Convolution time per repetition (in seconds)

### Hardware Details

The relevant details of the hardware used are:
- **CPU**: [Intel Core i7-12650H](https://www.intel.com/content/www/us/en/products/sku/226066/intel-core-i712650h-processor-24m-cache-up-to-4-70-ghz/specifications.html "Specification page for Intel Core i7-12650H"), 2.30 GHz up to 4.70 GHz, 10 cores (6 Performance, 4 Efficient), 16 threads, 24 MB of L3 cache
- **RAM**: DDR5 (4800 MHz), 16 GB
- **OS**: Windows 11 Home

## Further Checks

### Unit Testing

To ensure that both sequential and parallel versions work, the application code is supported by unit tests. Tests are written using the [GoogleTest](https://github.com/google/googletest "GitHub repository of GoogleTest") framework, configured in the `CMakeLists.txt` located in the [tests](./tests) folder:
- entities' tests (**PixelTest**, **ImageTest** and **KernelTest**) are quite simple and only check the constructor or default constructor behaviour.

  > :pencil: **Note**: Assertions uses `EXPECT_EQ` if its failure doesn't affect subsequent tests, or `ASSERT_EQ` if its truthfulness is necessary for the next ones.

- tests for the kernels building (**KernelFactoryTest**) check for kernel size oddity, and thus that the construction of the kernel respects its type. Since tests for different orders differ only in their values, parameterized tests are used here.

- image processing algorithms contain the most important logic to test (**ImageProcessingTest**):
  * for `extendEdge`, it is checked that the value of both the internal pixels and the new pixels on the edges is correct.
  * for `convolution`, the pixel values ​​of the transformed image are calculated by hand through the formula defined in the [Introduction](#introduction); some tests forces the transformed image to have out-of-range values for pixels, so that they can check if in-range convertion works.
  
  Tests for both functions also verify that the returned image is new.

- testing a third-party library is a bad practice in unit testing, that's why *stb* functions are wrapped in custom classes, i.e. facade design pattern is applied:
  * tests for loading use a simple and well-known JPG image to check if expected values are retrived from the image through the library.
  * tests for saving only check whether a JPG image file is created after the library call (without checking whether values are correct, because reading the contents would rely on the library itself).
  
  Tests for both methods also verify that an exception is thrown if the path is incorrect.

### Address Sanitization

In the main `CMakeLists.txt`, a few lines of configuration code are inserted to enable the [Google AddressSanitizer](https://github.com/google/sanitizers/wiki/addresssanitizer "GitHub Repository of ASan") (ASan) tool to check for memory issues:
```
if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC" AND USE_ASAN)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreadedDLL")
    add_compile_options(/fsanitize=address /Zi)
endif()
```
Currently, on Windows, this tool works well only with the MSVC compiler. Even though this project is built with LLVM/Clang, switching to MSVC does not require any adjustments other than changing the compiler itself.<br>

ASan instruments the code and generates an executable that replaces the allocation/deallocation methods with their equivalent methods. This way, memory issues are recognized and errors are printed when the executable is run. However, the overhead introduced in the program grows with the complexity of the code and increases the execution time. Since it is not always needed, the tool is only activated when the CMake option `-DUSE_ASAN=ON` is set in the used profile (e.g. Debug, Release).

### Profiling

To better understand where the program spends most of the time and to identify any bottlenecks, the [Intel VTune](https://www.intel.com/content/www/us/en/developer/tools/oneapi/vtune-profiler.html "Intel VTune website") code profiler is used. This tool provides statistical data on CPU usage, for this reason it is important not to consider the data as totally true and to reduce any non-deterministic effects by not using the machine during the measurements.

#### What to Profile

Profiling the main program is not a good idea because there are external components (e.g. timer) that are not of interest; also profiling each experiment singularly is not a good idea because operations are similar. For all those reasons, the `profile.cpp` contains a minimal test case defined by sufficiently large data, i.e. a single convolution of an image 6000x4000 pixels (`6K-1`) with a box blur kernel of order 19.

Profiling requires also to take into account how the application has been compiled: *debug* mode allows a better association between collected metrics and the source code, but the real program is obtained by the optimized *release* mode. Luckily, CMake allows to compile using the `RelWithDebInfo` mode, i.e. a fast enough version which includes debug informations, through the compiler options `-O2 -g -DNDEBUG`.

#### Results

As expected, the profiling shows that more than 50% of the execution is located in the `ImageProcessing::convolution` function, while approximately 25% of CPU work is necessary to pixel retrieval and destruction via the Pixel class. [Fig. 1](#figure-1) shows also the percentage of *retired instructions*. For Pixel's methods this rate is very low, and this contributes to the overhead; in this regard, a better definition or use of the class itself could bring benefits.

<p align="center">
  <img id="figure-1" src="/../assets/vtune_seq_rel_hs_1ms.png" alt="Screenshot of hotspot profiling results." title="Hotspot results" width="70%"/>
</p>

Another relevant outcome concerns *memory accesses*: no LLC (*last-level cache*) misses are detected with a CPU sampling interval of only 5ms! This is a positive result, but it must be verified by increasing the sampling rate: in fact, at 1ms the analysis detects more than 1 million LLC misses over 34s of CPU execution. Considering the size of the profiling test is not that little (there are 90 billion stores and 42 billion loads), LLC misses are not a problem for this project. Let's do some calculations:
- the LLC miss rate over all memory accesses is $1.150.000 / 132·10^9 \approx 8.71·10^{-6} = 0,000871\\%$
- there are $1.150.000 / 34 \approx 33.823$ misses per second
- for cache lines of $64 B$, the total traffic is $1.150.000 × 64 B = 73.600.000 B \approx 73.6 MB \sim 70.2 MiB$

This is equal to $\sim 2.0 MiB/s$ of DRAM average traffic due to LLC misses. In the worst case, the latency for each miss is about 300ns, therefore a loss of 0.345s wrt 34s of execution, i.e. about 1% of execution time, is a very low impact.



