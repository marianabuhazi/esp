# Test configs

## Overview

This directory contains a set of `.esp_config.bak.*` files to test the file generators found in `/esp/tools/socgen`. This directory also contains a couple of scripts to check the correctness of the file generators.

## Configurations

These configurations should be named as `esp_config.bak.<n>` where `<n>` is a number to be compatible with the testing scripts.

## Scripts

### `test_config.sh`

The usage of `test_config.sh` is
```
$ ./test_config.sh [OPTIONS] <soc> <n>
```
where `<soc>` is the directory name in `esp/socs/` (e.g. `xilinx-vc707-xc7vx485t`) and `<n>` is the number in the file `esp_config.bak.n`. The options available are `-h` for help and `-v` for verbose. The verbose flag will print out all the differences between the files, while omitting the verbose flag will only indicate which files are different.

This testing script clears the existing `.esp_config.bak.*` files in the parent directory, copies `.esp_config.bak.n` into the parent directory, and also copies `.esp_config.bak.n` into the directory `~/espcryo-ai/socs/<soc>/socgen/esp/`.

### `batch_test.sh`

The usage of `batch_test.sh` is
```
$ ./batch_test.sh <soc>
```
where `<soc>` is the directory name in `esp/socs/` (e.g. `xilinx-vc707-xc7vx485t`). This testing script runs `test_config.sh` multiple times to generate a table comparing all the files generated for each `.esp_config.bak.*` configuration file.
