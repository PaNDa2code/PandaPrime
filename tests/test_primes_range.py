from PandaPrimes import primes_range
import numpy as np
from os.path import join
from read_primes import PRIMES_LIST

def test_primes_range():
    primes = PRIMES_LIST()
    for index, prime in enumerate(primes_range(10**6)):
        assert primes[index] == prime

if __name__ == "__main__":
    test_primes_range()