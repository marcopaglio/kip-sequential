# kip-sequential - pixel_vector branch

This branch differs from the [main branch](https://github.com/marcopaglio/kip-sequential "Repository of kip-sequential's main branch") for the implementation of `data` attribute of `Image` class.<br>

In particular, the type `std::vector<std::vector<Pixel>>` is replaced with `std::vector<Pixel>`, that means reducing the use and convenience of the STL library in favor of faster execution speed.<br>

This involves in changing alghoritms that uses Image objects, especially pairs of square brackets used to access Pixel objects are substituted with single square brackets with more address logic. For example, `Pixel originalPixel = originalData[y + j][x + i];` becomes `Pixel originalPixel = originalData[(y + j) * width + (x + i)];`, i.e. the logic of data access is less clear.

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
      <td>1.55521</td>
      <td>1.54578</td>
      <td>1.53193</td>
      <td>2.72468</td>
      <td>2.88268</td>
      <td>2.81572</td>
      <td>4.58845</td>
      <td>4.41893</td>
      <td>4.52038</td>
      <td>6.57797</td>
      <td>6.47394</td>
      <td>6.67894</td>
    </tr>
    <tr>
      <td><strong>13</strong></td>
      <td>3.95338</td>
      <td>4.15121</td>
      <td>4.11401</td>
      <td>7.68928</td>
      <td>7.65376</td>
      <td>7.68323</td>
      <td>12.2896</td>
      <td>12.2677</td>
      <td>12.1722</td>
      <td>17.8716</td>
      <td>17.5752</td>
      <td>17.7598</td>
    </tr>
    <tr>
      <td><strong>19</strong></td>
      <td>7.79068</td>
      <td>7.77253</td>
      <td>7.80776</td>
      <td>14.3611</td>
      <td>14.4481</td>
      <td>14.4912</td>
      <td>23.1131</td>
      <td>23.0059</td>
      <td>23.1438</td>
      <td>31.8828</td>
      <td>33.3426</td>
      <td>33.7172</td>
    </tr>
    <tr>
      <td><strong>25</strong></td>
      <td>12.5505</td>
      <td>12.4788</td>
      <td>12.5083</td>
      <td>23.3414</td>
      <td>23.1048</td>
      <td>23.2913</td>
      <td>37.4235</td>
      <td>37.3231</td>
      <td>36.9943</td>
      <td>54.3418</td>
      <td>54.3019</td>
      <td>54.2909</td>
    </tr>
  </tbody>
</table>

It can be seen that the times recorded for inputs of the same size are very similar, indicating that the execution times of the operations are independent of the pixel values.
