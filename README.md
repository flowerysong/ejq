This is a simple CLI wrapper around James Barford's
[easy-json](https://github.com/Jamesbarford/easy-json) library.

```
$ hyperfine 'jq -r .recipients[] tfile' 'ejq -r -f tfile -e .recipients' -N --warmup 10 --runs 5000 -u millisecond
Benchmark 1: jq -r .recipients[] tfile
  Time (mean ± σ):       3.4 ms ±   1.4 ms    [User: 1.7 ms, System: 0.9 ms]
  Range (min … max):     2.5 ms …  24.3 ms    5000 runs

Benchmark 2: ejq -r -f tfile -e .recipients
  Time (mean ± σ):       0.7 ms ±   0.3 ms    [User: 0.4 ms, System: 0.2 ms]
  Range (min … max):     0.6 ms …   9.3 ms    5000 runs

Summary
  ejq -r -f tfile -e .recipients ran
    5.21 ± 3.12 times faster than jq -r .recipients[] tfile
```
