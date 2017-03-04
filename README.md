# spin-lock-tests
A few spin lock tests in N(1 2 4 6 8) threads on `Intel(R) Core(TM) i5-4258U CPU @ 2.40GHz` 
#Purpose

Inspired by [http://locklessinc.com/articles/locks/](http://locklessinc.com/articles/locks/) and [https://github.com/cyfdecyf/spinlock](https://github.com/cyfdecyf/spinlock) to learn spink lock.

#Usage

```bash
git clone https://github.com/detailyang/spin-lock-tests
make && make test
```

#Data
Test on my laptop with Intel(R) Core(TM) i5-4258U CPU @ 2.40GHz at Centos7
```
cas     0.089229	0.225248	0.803549	2.642590	7.408158
xchg    0.063846	0.163886	0.746205	2.522681	7.711956
xchg-backoff0.099984	0.175182	0.556483	4.555074	35.235432
mcs     0.059070	0.222442	0.482129	1.226916	6.217984
k42     0.108394	0.300572	0.475721	1.280443	4.767354
```
