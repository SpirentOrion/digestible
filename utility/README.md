#Digestible Utility

A utility program to assist with `Digestible` integration.

It's not always obvious what combination of `compression factor` as well as data types for input `values` and `weights` will yield the best speed/size compromise. To assist with this process use the `digestible-utility`.

`digestible-utility` has four modes of operation:
1. Suggest a compression factor.
2. Evaluate error percentages.
3. Verify error range for a given maximum percent error.
4. Benchmark time required to insert values into a `digestible` structure.

####Suggest a compression factor
This is the best place to start when first integrating a `digestible` structure into a project. Supply a minimum and maximum value, maximum acceptable error percentage, and a data type to use for `values`. Utility will run several iterations and return a recommended compression factor.

####Evaluate error percentages
This mode allows developers to get an idea of error percentages for a given compression factor. This is useful in cases where size considerations are of paramount importance.

####Verify error range for a given maximum percent error
Use this mode to get an idea of how often a given percent error and compression factor yield a positive result. As in how often the compression factor yields a percent error that's less than or equal to the provided one.

####Benchmark time required to insert values into a `digestible` structure
For an idea of how fast the structure can process input data use this mode. For example, during development it was observed that using `unsigned long long` as the `value` type yielded faster input performance than using `double`.

###Building
To build the utility:
```bash
cd utility
make
```

Utility has been successfully build on `Ubuntu 18.04 LTS` and `darwin 18 (aka macOS Mojave)`


###Examples
