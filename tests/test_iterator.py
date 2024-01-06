from PandaPrimes import Iterator
import numpy
from os.path import join
from read_primes import PRIMES_LIST

def test_Iterator():
    primes = PRIMES_LIST()
    it = Iterator()
    for i in range(len(primes)):
        assert primes[i] == it.next_prime()
    for i in range(0,len(primes),-1):
        assert primes[i] == it.prev_prime()

    it.jump_to(47)
    assert it.next_prime() == 47
    assert it.prev_prime() == 43

    it.jump_to(4180410070769835979)

    assert it.next_prime() == 4180410070769835979
    assert it.next_prime() == 4180410070769836039

if __name__ == "__main__":
    test_Iterator()