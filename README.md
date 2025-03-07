# kip-sequential

kip-sequential is the sequential version of Kernel Image Processing, that is kernel filtering applied to 2D images.<br>
Parallelized versions of kip-sequential can be found at:

- [kip-parallel-openMP](https://github.com/marcopaglio/kip-parallel-openMP "Repository of kip-parallel-openMP")
- [kip-parallel-CUDA](TODO "Repository of kip-parallel-CUDA")

## Introduction

Kernel Image Processing is used to enhance, filter, and analyze images. It involves applying a small matrix, known as a **kernel**, **filter** or **mask**, to an image to perform transformations, such as blurring, sharpening, embossing, edge detection, and more. This is accomplished by doing a **convolution** between the kernel and an image, that is:

1. The kernel moves across the image pixel by pixel.
2. At each position, it multiplies its values with the corresponding pixel values in the image.
3. The results are summed up and replace the central pixel.
4. This process repeats for all pixels, generating a modified image.

<p align="center">
  <img src="/../assets/2D_Convolution_Animation.gif" alt="Screenshot of the simple BookingApp GUI." title="BookingApp GUI" width="30%"/>
</p>

TODO: math operation, then algorithm, then implementation
