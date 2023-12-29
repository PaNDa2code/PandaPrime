# PandaPrimes

PandaPrimes is a CPython extension under active development, leveraging the powerful [primesieve](https://github.com/kimwalisch/primesieve) library to significantly enhance prime number generation performance.

## Usage

### Iterating Over Primes
Iterating through primes is a common practice, providing a memory-efficient way to work with prime numbers. PandaPrimes introduces two main functionalities for iterating through primes.

#### `primes_range`

The `primes_range` function enables seamless iteration through prime numbers within a specified range.

```python
from PandaPrimes import primes_range

primes_count = 0

# Example 1: Iterate through primes less than or equal to one million
for prime in primes_range(10**6):
    primes_count += 1
```

```python
primes_count = 0

# Example 2: Iterate through primes in the range between one million and five million
for prime in primes_range(10**6, 5*10**6):
    primes_count += 1
```

### Prime Generation

#### `generate_primes`

The `generate_primes` function creates a NumPy array containing prime numbers within a specified range.

```python
from PandaPrimes import generate_primes

# Example: Generate an array of primes from 2 to 1e10
primes_array = generate_primes(10**10)
```

#### `generate_n_primes`

The `generate_n_primes` function generates a NumPy array of the first `n` prime numbers.

```python
from PandaPrimes import generate_n_primes

# Example: Generate an array containing the first million prime numbers
primes_array = generate_n_primes(10**6)
```

## Installation

As of now, PandaPrimes is still under development.

- for now you can try it by installing primesieve library on you machine first. [install primesieve](https://github.com/kimwalisch/primesieve#installation)

- after that you can simply install PandaPrimes using pip.
```bash
pip install PandaPrimes
```

## Contributing

We welcome contributions from the community! Feel free to contribute by opening issues, submitting pull requests, or providing feedback.

Please note that the README usage section is a work in progress, and further details will be added as development progresses.
