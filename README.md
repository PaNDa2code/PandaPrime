# PandaPrime

PandaPrime is a CPython extension still under development, based on the excellent [prime_sieve](https://github.com/kimwalisch/primesieve), providing a significant performance boost.

## Usage

### `primes_range`

The `primes_range` function allows you to loop through prime numbers within a specified range.

```python
from PandaPrime import primes_range

# Example: Loop through primes less than or equal to one million
for prime in primes_range(1e6):
    print(prime, end=" ")
```
## Installation
Currently, PandaPrime is under development, and installation steps will be provided upon release.

## Contributing
Feel free to contribute by opening issues or submitting pull requests.