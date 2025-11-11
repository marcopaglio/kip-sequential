## Kip-sequential - SoA

This is the *Structure-of-Arrays* version of **kip-sequential**. Alternative versions can be found at:

- [AoS folder](../AoS "AoS version of kip-sequential")
- [Higher-level-AoS branch](/../edgeHandler_strategy "Alternative and higher-level AoS version of kip-sequential")
- [Lower-level-AoS branch](/../pixel_vector "Alternative and lower-level AoS version of kip-sequential")

### Implementation Differences

This version differs from the:

- [AoS version](../AoS "AoS version of kip-sequential") for the removal of Pixel class and the substitution of `data` attribute of `Image` class with 3 other vectors: `reds`, `greens`, and `blues`, each of which is of type `std::vector<uint8_t>`.<br>
- [pixel_vector branch](https://github.com/marcopaglio/kip-sequential/tree/pixel_vector "Repository of kip-sequential's pixel_vector branch") for the transition from a Array-of-Structures (AoS) to a Structure-of-Arrays (SoA) to contain data in the `Image` class without the need for the `Pixel` class.<br>

This involves in changing alghoritms that creates and uses Image objects, especially pairs of square brackets used to access Pixel objects are substituted with single square brackets with more address logic. For example, `Pixel originalPixel = originalData[y + j][x + i];` now becomes:
```
uint8_t red = originalReds[(y + j) * width + (x + i)];
uint8_t green = originalGreens[(y + j) * width + (x + i)];
uint8_t blue = originalBlues[(y + j) * width + (x + i)];
```
i.e. the logic of data access and algorithm is much less clear.

### Experimental Results

SoA undoubtedly makes algorithms much faster.

TODO

### Profiling Results

TODO
