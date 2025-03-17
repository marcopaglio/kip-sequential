# kip-sequential

This is the *sequential* version of **Kernel Image Processing**, which is a convolution-based filtering applied to 2D images using a variable size kernel. Parallelized versions of kip-sequential can be found at:

- [kip-parallel-openMP](https://github.com/marcopaglio/kip-parallel-openMP "Repository of kip-parallel-openMP")
- [kip-parallel-CUDA](TODO "Repository of kip-parallel-CUDA")

## Introduction

Kernel Image Processing is used to enhance, filter, and analyze images. It involves applying a small squared matrix of odd order, known as a **kernel**, to a 2D image to perform transformations, such as blurring, sharpening, embossing, edge detection, and more. This is accomplished by doing a **convolution** between the kernel and an image, that is:

1. The kernel moves across the image pixel by pixel.
2. At each position, it multiplies its values with the corresponding pixel values in the image.
3. The results are summed up and replace the central pixel.
4. This process is repeated for all pixels, generating a transformed image.

<p align="center">
  <img src="/../assets/2D_Convolution_Animation.gif" alt="Screenshot of the simple BookingApp GUI." title="BookingApp GUI" width="30%"/>
</p>

The general expression of the convolution of an image $I$ with a kernel $K$ is defined as:<br>

$$I'(x, y) = \sum_{m=-k}^k \sum_{n=-k}^k K(m, n) \cdot I(x+m, y+n) $$

where $x$ and $y$ are the coordinates of the pixel in the 2D image, $I'$ is the transformed image, and every element of $K$ is considered by $-k \leq m \leq k$ and $-k \leq n \leq k$, where $k$ is half the kernel size (approximated by default, e.g. for a 3x3 kernel, k = 1).

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

Kernel convolution requires values from pixels outside of the image boundaries; there are a variety of methods for handling image edges:

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
  Example 3x3:  
                  [ 0  0  0 ]
                  [ 0  1  0 ]
                  [ 0  0  0 ]
  ```
- **Box Blur**: a simple averaging filter that replaces each pixel with the mean of its neighbors.
  ```
  Example 3x3:  
                  [ 1  1  1 ]
              1/9 [ 1  1  1 ]
                  [ 1  1  1 ]
  ```
- **Edge Detection**: highlight changes in intensity, useful for detecting shapes and boundaries.
  ```
  Example 3x3:  
                  [ -1  -1  -1 ]
                  [ -1   8  -1 ]
                  [ -1  -1  -1 ]
  ```
- **Sharpen**: enhance edges and details by amplifying differences.
  ```
  Example 3x3:  
                  [  0  -1   0 ]
                  [ -1   5  -1 ]
                  [  0  -1   0 ]
  ```

## Implementation

TODO
