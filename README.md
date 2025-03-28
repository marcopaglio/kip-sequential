# kip-sequential - pixel_SoA branch

## Differences

This branch differs from the:

- [pixel_vector branch](https://github.com/marcopaglio/kip-sequential/tree/pixel_vector "Repository of kip-sequential's pixel_vector branch") for the transition from a Array-of-Structures (AoS) to a Structure-of-Arrays (SoA) to contain data in the `Image` class without the need for the `Pixel` class.<br>
- [main branch](https://github.com/marcopaglio/kip-sequential "Repository of kip-sequential's main branch") for the removal of Pixel class and the substitution of `data` attribute of `Image` class with 3 other vectors: `reds`, `greens`, and `blues`, each of which is of type `std::vector<uint8_t>`.<br>

This involves in changing alghoritms that creates and uses Image objects, especially pairs of square brackets used to access Pixel objects are substituted with single square brackets with more address logic. For example, `Pixel originalPixel = originalData[y + j][x + i];` now becomes:
```
uint8_t red = originalReds[(y + j) * width + (x + i)];
uint8_t green = originalGreens[(y + j) * width + (x + i)];
uint8_t blue = originalBlues[(y + j) * width + (x + i)];
```
i.e. the logic of data access and algorithm is much less clear.

## Benefits

SoA undoubtedly makes algorithms much faster.
