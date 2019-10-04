
# digestible [![CircleCI](https://circleci.com/gh/SpirentOrion/digestible.svg?style=svg)](https://circleci.com/gh/SpirentOrion/digestible)

A modern C++ implementation of a merging `t-digest` data structure.

`t-digests` are data structures that provide highly accurate statistics for a large number of inputs in a small memory footprint. Things like minimum, maximum, average, quantile, and continuous distribution are readily calculated. Think improved histogram: compact way of accurately summarizing data where the bucket sizes need not be set in advance.

##### Notable features include:
* No runtime memory allocation.
* Strict, user-defined upper bound on structure size.
* Written in modern C++ (c++17).
* Templated `mean` and `weight` data types - save memory for smaller input ranges with a smaller data type.
* Speed (average 61ns insertion time per element with a c. 2019 Intel i7)

##### Requirements:
* A C++17 compatible compiler (developed with `clang 7` using `--std=c++17`).
* `CMake` (for building utility and running tests).

### Sample Code
```c++
#include "digestible/digestible.h"

using namespace std;
using namespace digestible;

// Structure will have at most 20 elements.
// Also default to values of type float and weights of type unsigned.
tdigest digest(20);

// Add some data into the structure.
digest.add(5);
digest.add(10);
digest.add(4);

// digestible buffers input data; explicitly merge it before querying statistics.
digest.merge();

printf("50th quantile is: %f\n", digest.quantile(50));

```
Additional examples can be found in the [`tests`](tests) directory.

### `digestible` Utility
To assist in integrating `digestible` in your implementation, please see [`utility`](utility) directory.

### Compression Factor
As the lone parameter to the `digestible` constructor, __compression factor__ requires some discussion. Internally `t-digests` use the _compression factor_ to distribute values throughout the input range. When combined with an internal _distribution function_ a `t-digest` will distribute data such that the head and tail have fewer points and the middle has significantly more. Higher _compression factors_ will yield better accuracy at the expense of increased memory usage. The `digestible utility` can be used to help strike this balance.

### Data Structure Size
For a given compression factor `f`, value type size `v`, and weight type size `w`, the approximate `t-digest` size in bytes is: 
```
f * max(v, w) * 8 + 64
```

### Acknowledgments
This work would not be possible without the original algorithm and reference implementation by [tdunning](https://github.com/tdunning/t-digest). A local copy of the original `t-digest` paper can be found [here](reference/Computing%20Extremely%20Accurate%20Quantiles%20Using%20t-Digests.pdf).
