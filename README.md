# spin-lock-tests [![Build](https://api.travis-ci.org/detailyang/spin-lock-tests.svg)](https://travis-ci.org/detailyang/spin-lock-tests)
A few spin lock tests in N(1 2 4 6 8 16) threads on `Intel(R) Core(TM) i5-4258U CPU @ 2.40GHz` 

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
cas                     0.089129	0.179701	0.600171	1.949648	7.880399
xchg                    0.056529	0.121979	0.589100	2.243223	7.206136
xchg-backoff            0.084343	0.167522	0.924299	2.786340	40.560166
mcs                     0.073797	0.125819	0.423952	1.217162	6.716630
k42                     0.162412	0.241256	0.444286	1.850383	5.780539
```

Contributing
------------

To contribute to spin-lock-tests, clone this repo locally and commit your code on a separate branch. 


Author
------

> GitHub [@detailyang](https://github.com/detailyang)     


License
-------

spin-lock-tests is licensed under the [MIT](https://github.com/detailyang/spin-lock-tests/blob/master/LICENSE) license.  
