from PandaPrimes import *

def test_get_nth_prime():
    assert get_nth_prime(10**9) == 22801763489
    assert get_nth_prime(10**9,10**6) == 22803640037
    assert get_nth_prime(10**9,10**4) == 22801792709

def test_count_primes():
    assert count_primes(10**9) == 50847534
    assert count_primes(1,10**10) == 455052511

def test_count_twins():
    assert count_twins(10**9) == 3424506

def test_is_prime():
    assert is_prime(18446744073709551557)

test_count_primes()
