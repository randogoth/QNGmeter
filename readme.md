# QNGFreeMeter

A fork of [Comscire's QNGmeter Console](https://comscire.com/downloads/qngmeterdoc/) that does not require [libqwqng](https://comscire.com/downloads/) drivers or Comscire hardware but analyzes binary data passed to it via stdin pipe.

## Build

requires CMake and C++ compiler

```sh
$ cmake .
$ make
```

## Use

```
$ dd if=/dev/urandom of=random_data.bin bs=4096 count=4096
$ cat random_data.bin | ./qngmeter
```

## Output

```sh
     QNGmeter Console 1.0         Test Type      z-score   p[z<=x]  p[chi2<=x] 
 +---------------------------+------------------------------------------------+
 |                           |  1/0 Balance       -0.988    0.1615    0.8592  |
 |                           |  Serial Test       +0.000    0.5000    0.8463  |
 |                           |  OQSO Test         -0.395    0.3465    0.5839  |
 |                           |  Entropy Test      +1.011    0.8439    0.8463  |
 |                           |    H: 1.000060340                              |
 |                           |                                                |
 |                           |   1st  AutoCorr    +2.147    0.9841    0.0327  |
 |   Start Time              |   2st  AutoCorr    +0.561    0.7127    0.8802  |
 |     Mar  2 20:46:02       |   3st  AutoCorr    +0.332    0.6302    0.7652  |
 |                           |   4st  AutoCorr    +1.148    0.8746    0.7992  |
 |   Total Bits Tested       |   5st  AutoCorr    +0.539    0.7051    0.6883  |
 |      0.00e+00             |   6st  AutoCorr    -1.338    0.0904    0.0292  |
 |                           |   7st  AutoCorr    -0.595    0.2760    0.4126  |
 |   Throughput              |   8st  AutoCorr    -0.710    0.2388    0.2081  |
 |      0.0 Mbps             |   9st  AutoCorr    +0.893    0.8140    0.0309  |
 |                           |  10st  AutoCorr    -0.364    0.3578    0.7971  |
 |   Bits Tested Percent     |  11st  AutoCorr    -0.948    0.1715    0.3216  |
 |      -nan%                |  12st  AutoCorr    -0.881    0.1892    0.3113  |
 |                           |  13st  AutoCorr    +0.534    0.7035    0.3231  |
 |                           |  14st  AutoCorr    -0.385    0.3500    0.9147  |
 |                           |  15st  AutoCorr    +0.165    0.5655    0.9145  |
 |                           |  16st  AutoCorr    -0.028    0.4890    0.4693  |
 |                           |  17st  AutoCorr    +1.208    0.8865    0.1560  |
 |   Meta KS+ Test           |  18st  AutoCorr    +1.373    0.9151    0.8381  |
 |     0.000                 |  19st  AutoCorr    -0.154    0.4390    0.9732  |
 |                           |  20st  AutoCorr    -0.233    0.4079    0.1341  |
 |                           |  21st  AutoCorr    -1.723    0.0424    0.0645  |
 |   Meta KS- Test           |  22st  AutoCorr    -0.897    0.1849    0.5430  |
 |     0.000                 |  23st  AutoCorr    +0.806    0.7900    0.3512  |
 |                           |  24st  AutoCorr    -1.346    0.0891    0.7598  |
 |                           |  25st  AutoCorr    +0.142    0.5563    0.9886  |
 |                           |  26st  AutoCorr    -0.320    0.3745    0.2615  |
 |                           |  27st  AutoCorr    -0.157    0.4378    0.8338  |
 |                           |  28st  AutoCorr    +0.366    0.6429    0.4186  |
 |   QNGmeter Score          |  29st  AutoCorr    +1.361    0.9132    0.8088  |
 |      0.0+                 |  30st  AutoCorr    -0.278    0.3905    0.4151  |
 |                           |  31st  AutoCorr    -0.092    0.4634    0.2645  |
 |                           |  32st  AutoCorr    +0.574    0.7170    0.7009  |
 +---------------------------+------------------------------------------------+
```