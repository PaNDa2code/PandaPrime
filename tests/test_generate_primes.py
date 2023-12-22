from PandaPrimes import generate_primes, generate_n_primes
import numpy
from time import time
from sys import getsizeof

def _PRIME_LIST():
    path_to_primes = "/".join(__file__.split("/")[:-1]) + "/primes.txt"
    with open(path_to_primes,"r") as file:
        primes = numpy.array(list(map(int, file.read().split(", "))))
    return primes

def test_generate_primes():
    primes = _PRIME_LIST()
    assert numpy.array_equal(generate_primes(10**6), primes)
    
def test_n_generate_primes():
    n = 10000
    primes = _PRIME_LIST()
    assert numpy.array_equal(generate_n_primes(n),primes[:n])
    start = int(primes[n])
    assert numpy.array_equal(generate_n_primes(n,start), primes[n:n*2])
    

def generation_performance():
    started = time()
    primes = generate_primes(10**10)
    ended = time()
    size = getsizeof(primes) // (1024 ** 2)
    print(f"enrated primes from 2 to 10^10 in {(ended-started):.3f}s\nWith size of {size} MB")

if __name__ == "__main__":
    test_generate_primes()
    test_n_generate_primes()
    generation_performance()