from PandaPrimes import *

def test_get_nth_prime():
    assert get_nth_prime(10**9) == 22801763489
    assert get_nth_prime(10**9,10**6) == 22803640037
    assert get_nth_prime(10**9,10**4) == 22801792709

def test_count_primes():
    assert count_primes(10**9) == 50847534
    assert count_primes(1,10**10) == 455052511

