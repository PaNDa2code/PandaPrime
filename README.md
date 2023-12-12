# PandaPrime

panda prime is Cpython extension `still under development`.

**These module is based on the awesome <a href="https://github.com/kimwalisch/primesieve">prime_sieve</a> that gives a huge performance boost.**
#### `primes_range`:
```
from PandaPrime import primes_range

# loop through primes thats "less than or equle one million"
for prime in primes_range(1e6):
    print(prime, end=" ")
```
