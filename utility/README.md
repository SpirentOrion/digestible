# `digestible` Utility

A utility program to assist with `Digestible` integration.

It's not always obvious what combination of `compression factor` as well as data types for input `values` and `weights` will yield the best speed/size compromise. To assist with this process use the `digestible-utility`.

`digestible-utility` has four modes of operation:
1. Suggest a compression factor.
2. Evaluate error percentages.
3. Verify error range for a given maximum percent error.
4. Benchmark time required to insert values into a `digestible` structure.

#### Suggest a compression factor
This is the best place to start when first integrating a `digestible` structure into a project. Supply a minimum and maximum value, maximum acceptable error percentage, and a data type to use for `values`. Utility will run several iterations and return a recommended compression factor.

#### Evaluate error percentages
This mode allows developers to get an idea of error percentages for a given compression factor. This is useful in cases where size considerations are of paramount importance.

#### Verify error range for a given maximum percent error
Use this mode to get an idea of how often a given percent error and compression factor yield a positive result. As in how often the compression factor yields a percent error that's less than or equal to the provided one.

#### Benchmark time required to insert values into a `digestible` structure
For an idea of how fast the structure can process input data use this mode. For example, during development it was observed that using `unsigned long long` as the `value` type yielded faster input performance than using `double`.

## Building
To build the utility in a directory `build`:
```bash
cmake . -Bbuild
cmake --build build
```

Utility has been successfully build on `Ubuntu 18.04 LTS` and `darwin 18` (aka macOS Mojave).

## Examples
#### Determine a compression factor
Consider an integral input range of `[64, 16384]` and expecting at least `1,000,000` samples. Storing values as type `unsigned short int` and weights as `unsigned long long int`, the following command can be used to determine a compression factor.
```
utility/utility -s 64 -S 16384 -c 1000000 -e 6 -i integral -a "unsigned short int" -w "unsigned long long int" -m compression-factor -t 40
Test parameters:

Test Mode: compression factor
Minimum expected value: 64
Maximum expected value: 16384
Number of values to test with: 1000000
Maximum percent error: 6
Number of trials to run: 40
Value data type: unsigned short int
Weight data type: unsigned long long int
Input data type: integral

For 1000000 values in the range [64, 16384]:
For a percent error of not more than 6 percent, recommended compression factor: 16
Scale factor results min/avg/max  10/11.15/16

```

#### Check error percentages
Consider a case with a floating-point input range of `[0, 100]` expecting at least `30,000,000` samples. Storing values as `double` and weights as `unsigned long long int` and requiring a maximum data structure size on the order of `1024` bytes, what error percentages can be expected?
```
 utility/utility -s 0 -S 100 -c 30000000 -f 16 -i floating_point -a "double" -w "unsigned long long int" -m percent-error -t 40
Test parameters:

Test Mode: percent error
Minimum expected value: 0
Maximum expected value: 100
Number of values to test with: 30000000
Compression factor: 16
Number of trials to run: 40
Value data type: double
Weight data type: unsigned long long int
Input data type: floating_point

For 30000000 values in the range [0, 100], with a compression factor of 16:
Percent error results min/avg/max  8.02568/9.53181/10.5911 percent
```

#### Verify error range
Continuing the [previous example](#check-error-percentages), what's the probability that the maximum percent error is 9.5%?
```
utility/utility -s 0 -S 100 -c 30000000 -f 16 -i floating_point -a "double" -w "unsigned long long int" -m verify -e 9.5 -t 40
Test parameters:

Test Mode: verify
Minimum expected value: 0
Maximum expected value: 100
Number of values to test with: 30000000
Compression factor: 16
Maximum percent error: 9.5
Number of trials to run: 40
Value data type: double
Weight data type: unsigned long long int
Input data type: floating_point

For 30000000 values in the range [0, 100], with a compression factor of 16 and a maximum percent error of 9.5: 
Percent of trials that satisfied constraints: 62.5 percent
```

#### Benchmark
Consider the [first example](#determine-a-compression-factor), what sort of insertion performance can users expect?
```
utility/utility -s 64 -S 16384 -c 1000000 -e 6 -i integral -a "unsigned short int" -w "unsigned long long int" -m benchmark -f 16 -t 40 
Test parameters:

Test Mode: benchmark
Minimum expected value: 64
Maximum expected value: 16384
Number of values to test with: 1000000
Compression factor: 16
Maximum percent error: 6
Number of trials to run: 40
Value data type: unsigned short int
Weight data type: unsigned long long int
Input data type: integral

Time to insert 1000000 values in the range [64, 16384]:
Benchmark results min/avg/max 52.8089/55.4543/67.6084  ms
```

