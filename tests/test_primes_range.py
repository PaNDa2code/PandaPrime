from PandaPrimes import primes_range
import numpy
from os.path import join

def _PRIME_LIST():
    path_to_primes = join("/".join(__file__.split("/")[:-1]), "primes.txt")
    with open(path_to_primes,"r") as file:
        primes = numpy.array(list(map(int, file.read().split(", "))))
    return primes

def test_primes_range():
    primes = _PRIME_LIST()
    for index, prime in enumerate(primes_range(10**6)):
        assert primes[index] == prime

if __name__ == "__main__":
    test_primes_range()