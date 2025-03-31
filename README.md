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

The implementation is intentionally kept simple:

- the three entities (**Pixel**, **Image** and **Kernel**) are read only: they have no setter or other modifier methods, so how they remain as when they are built with the constructor. As a consequence, image processing's functions have to instantiate new objects instead of modififying the existing ones.
- pixels in the image are stored as vectors of vectors, i.e. `vector<vector<Pixel>>`, in order to access the elements of the image with a matrix. This involves in lots of overhead because the use of STL, but code is clearer. Alternative versions of this data structure are proposed in [pixel_vector](/../pixel_vector) and [pixel_SoA](/../pixel_SoA) branches, where pixels are stored as a unique vector, i.e. `vector<Pixel>`, and three simple vectors of red, green and blue values, i.e. `vector<uint_8>`, respectively.
- there is no need to have multiple kernel type classes because they all behave the same way, but the way they are constructed depends on their type. For this reason, **KernelFactory** has a static method for each kernel type which builds the kernel with the appropriate values based on its order.
- to read pixel values from images (JPG, PNG, etc) and to save the transformed image into a JPG (or similarly in other formats) images, an external library is used: [**stb**](https://github.com/nothings/stb "Github repository of stb") . It just needs to include in the project its two header files (`stb_image.h`, `stb_image_write.h`), and then to use the following definitions and inclusions in the code where it is used:
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
  

### TEST

### Misurazione del tempo (struttura MAIN)
