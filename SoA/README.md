## Kip-sequential - SoA

This is the *Structure-of-Arrays* version of **kip-sequential**. Alternative versions can be found at:

- [AoS folder](../AoS "AoS version of kip-sequential")
- [Higher-level-AoS branch](https://github.com/marcopaglio/kip-sequential/tree/edgeHandler_strategy "Alternative and higher-level AoS version of kip-sequential")
- [Lower-level-AoS branch](https://github.com/marcopaglio/kip-sequential/tree/pixel_vector "Alternative and lower-level AoS version of kip-sequential")

### Implementation Differences

This version differs from the:

- [pixel_vector branch](https://github.com/marcopaglio/kip-sequential/tree/pixel_vector "Repository of kip-sequential's pixel_vector branch") for the transition from a Array-of-Structures (AoS) to a Structure-of-Arrays (SoA) to contain data in the `Image` class without the need for the `Pixel` class.<br>
- [AoS version](../AoS "AoS version of kip-sequential") for the removal of Pixel class and the substitution of `data` attribute of `Image` class with 3 other vectors: `reds`, `greens`, and `blues`, each of which is of type `std::vector<uint8_t>`.<br>

This involves in changing alghoritms that creates and uses Image objects, especially pairs of square brackets used to access Pixel objects are substituted with single square brackets with more address logic. For example, `Pixel originalPixel = originalData[y + j][x + i];` now becomes:
```
uint8_t red = originalReds[(y + j) * width + (x + i)];
uint8_t green = originalGreens[(y + j) * width + (x + i)];
uint8_t blue = originalBlues[(y + j) * width + (x + i)];
```
i.e. the logic of data access and algorithm is much less clear.

### Experimental Results

The following tables summarizes the temporal measurements of convolutions on different images with different kernels, measured in release mode.

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
      <td>0.442286</td>
      <td>0.387787</td>
      <td>0.388366</td>
      <td>0.743224</td>
      <td>0.721584</td>
      <td>0.721114</td>
      <td>1.15274</td>
      <td>1.1539</td>
      <td>1.1652</td>
      <td>1.68417</td>
      <td>1.68051</td>
      <td>1.69626</td>
    </tr>
    <tr>
      <td><strong>13</strong></td>
      <td>1.2938</td>
      <td>1.25983</td>
      <td>1.28512</td>
      <td>2.35938</td>
      <td>2.35423</td>
      <td>2.3673</td>
      <td>3.80332</td>
      <td>3.78081</td>
      <td>3.81567</td>
      <td>5.5323</td>
      <td>5.50108</td>
      <td>5.51086</td>
    </tr>
    <tr>
      <td><strong>19</strong></td>
      <td>2.70637</td>
      <td>2.69978</td>
      <td>2.70756</td>
      <td>5.02448</td>
      <td>5.01598</td>
      <td>5.02325</td>
      <td>8.00071</td>
      <td>8.00591</td>
      <td>8.03306</td>
      <td>11.6609</td>
      <td>11.6833</td>
      <td>11.6808</td>
    </tr>
    <tr>
      <td><strong>25</strong></td>
      <td>4.65149</td>
      <td>4.61485</td>
      <td>4.64684</td>
      <td>8.62884</td>
      <td>8.63837</td>
      <td>8.63174</td>
      <td>13.827</td>
      <td>13.807</td>
      <td>13.7959</td>
      <td>20.0536</td>
      <td>20.0431</td>
      <td>20.0628</td>
    </tr>
  </tbody>
</table>

Just as in the AoS version, it can be seen that recorded times for inputs of the same size are very similar, indicating that the execution times of the operations are independent of the pixel values.<br/>
Furthermore, the SoA version undoubtedly makes algorithms much faster: there is a performance gain ranging from at least 2.75x to over 4x.

### Profiling Results

The profiling analysis for the SoA version of the Kernel Image Processing has been maken directly with a sampling rate of 1ms for two reasons:
1. To compare results with the AoS version.
2. Cause the execution time is much faster than AoS it would not have made sense to use a coarser sampling rate.

The profiling shows that the program is heavily dominated by the function **`ImageProcessing::convolution`** which is clearly the application’s main hotspot. In the Hotspots analysis, this function accounts for approximately **9.38 s of CPU time**, while the other functions appearing among the top results mainly belong to the JPEG image handling libraries (i.e. `stb_image` and `stb_image_write`) and the Windows runtime libraries, as shown in the [Fig. 2](#figure-2). This highlights that convolution is the main area of focus for optimising the program.

<p align="center">
  <img id="figure-2" src="/../assets/vtune_seq_rel_hs_1ms_soa.png" alt="Screenshot of hotspot profiling results." title="Hotspot results" width="70%"/>
</p>

The *Memory Access* analysis for the `ImageProcessing::convolution` function shows very favourable behaviour (see [Fig. 3](#figure-3)):
- The most significant finding is the **Memory Bound of 2.1%**: only a relatively small fraction of the pipeline’s capacity is lost due to memory-related constraints.
- The number of **loads** and **stores** performed by the function is **21.5** and **0.4 billion**, respectively, which is significantly lower than the figures recorded in the same analysis for the AoS version.
- The number of **LLC misses** is approximately **55,000**, which means that the ratio is in the order of a few misses per tens or hundreds of thousands of loads. Consequently, memory accesses are served by the nearest levels of the cache hierarchy, without frequently having to resort to the last level of the cache.

<p align="center">
  <img id="figure-3" src="/../assets/vtune_seq_rel_macc_1ms_soa.png" alt="Screenshot of memory access profiling results." title="Memory Access results" width="70%"/>
</p>

Finally, among the results for the *Microarchitecture Exploration* analysis, the **73.2% Retiring** figure stands out: the vast majority of pipeline resources are used to complete instructions that are actually useful, rather than being wasted due to **Front-End Bound** (**3.2%**) or **Bad Speculation** (**2.4%**).<br/>
The same cannot be said for **Back-End Bound**, which stands at **21.2%**. It represents the main component of pipeline stalls; however:
- only 2.2% is due to *Memory Bound*;
- the remaining 19.3% is due to *Core Bound*, potentially attributable to the intrinsic nature of the algorithm.




