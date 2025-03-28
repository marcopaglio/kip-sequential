# kip-sequential - pixel_vector branch

This branch differs from the [main branch](https://github.com/marcopaglio/kip-sequential "Repository of kip-sequential's main branch") for the implementation of `data` attribute of `Image` class.<br>

In particular, the type `std::vector<std::vector<Pixel>>` is replaced with `std::vector<Pixel>`, that means reducing the use and convenience of the STL library in favor of faster execution speed.<br>

This involves in changing alghoritms that uses Image objects, especially pairs of square brackets used to access Pixel objects are substituted with single square brackets with more address logic. For example, `Pixel originalPixel = originalData[y + j][x + i];` becomes `Pixel originalPixel = originalData[(y + j) * width + (x + i)];`, i.e. the logic of data access is less clear.
