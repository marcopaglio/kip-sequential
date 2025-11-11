## Kip-sequential - AoS

This is the *Array-of-Structures* version of **kip-sequential**. Alternative versions can be found at:

- [SoA folder](./SoA "SoA version of kip-sequential")
- [Higher-level-AoS branch](/../edgeHandler_strategy "Alternative and higher-level AoS version of kip-sequential")
- [Lower-level-AoS branch](/../pixel_vector "Alternative and lower-level AoS version of kip-sequential")

### Implementation Differences

The kip-sequential structure of the AoS implementation is fully described in the Section [Implementation Details](../README.md#implementation-details "Implementation Details section of the main README file").

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
  </tbody>
</table>

### Profiling Results

The profiling shows that more than 50% of the execution is located in the `ImageProcessing::convolution` function, while approximately 25% of CPU work is necessary to pixel retrieval and destruction via the Pixel class. [Fig. 1](#figure-1) shows also the percentage of *retired instructions*. For Pixel's methods this rate is very low, and this contributes to the overhead; in this regard, a better definition or use of the class itself could bring benefits.

<p align="center">
  <img id="figure-1" src="/../assets/vtune_seq_rel_hs_1ms.png" alt="Screenshot of hotspot profiling results." title="Hotspot results" width="70%"/>
</p>

Another relevant outcome concerns *memory accesses*: no LLC (*last-level cache*) misses are detected with a CPU sampling interval of only 5ms! This is a positive result, but it must be verified by increasing the sampling rate: in fact, at 1ms the analysis detects more than 1 million LLC misses over 34s of CPU execution. Considering the size of the profiling test is not that little (there are 90 billion stores and 42 billion loads), LLC misses are not a problem for this project. Let's do some calculations:
- the LLC miss rate over all memory accesses is $1.150.000 / 132·10^9 \approx 8.71·10^{-6} = 0,000871\\%$
- there are $1.150.000 / 34 \approx 33.823$ misses per second
- for cache lines of $64 B$, the total traffic is $1.150.000 × 64 B = 73.600.000 B \approx 73.6 MB \sim 70.2 MiB$

This is equal to $\sim 2.0 MiB/s$ of DRAM average traffic due to LLC misses. In the worst case, the latency for each miss is about 300ns, therefore a loss of 0.345s wrt 34s of execution, i.e. about 1% of execution time, is a very low impact.
