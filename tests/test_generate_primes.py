from PandaPrimes import generate_primes, generate_n_primes
import numpy
from time import time
from sys import getsizeof
from os.path import join
from read_primes import PRIMES_LIST

def test_generate_primes():
    primes = PRIMES_LIST()
    assert numpy.array_equal(generate_primes(10**6), primes)
    
def test_n_generate_primes():
    n = 10000
    primes = PRIMES_LIST()
    assert numpy.array_equal(generate_n_primes(n),primes[:n])
    start = int(primes[n])
    assert numpy.array_equal(generate_n_primes(n,start), primes[n:n*2])
    

if __name__ == "__main__":
    test_generate_primes()
    test_n_generate_primes()