from . import PandaPrimes as _PP
import numpy as np
from typing import Union

def generate_primes(start: Union[int, float], end: Union[int, float] = 0) -> np.ndarray:
    """
    Generate an array of prime numbers in the specified range.

    Args:
        start (int): The starting value of the range.
        end (int, optional): The ending value of the range. If not provided, it defaults to 0 and the start will be used instead.

    Returns:
        np.ndarray[int]: NumPy array containing prime numbers.

    Example:
        ```
        primes_array = generate_primes(10, 50)
        print(primes_array)
        ```
        ```
        primes_array = generate_primes(10**6)
        print(primes_array)
        """
    start, end = int(min(start, end)), int(max(start, end))
    return _PP.generate_primes(start, end)

def generate_n_primes(n: Union[int, float], start: int = 0) -> np.ndarray:
    """
    Generate an array of the first n prime numbers.

    Args:
        n (int): The ending value to limit the count. If not provided, it defaults to 0.
        start (int, optional): The starting value to count the first n primes.

    Returns:
        np.ndarray[int]: NumPy array containing the first n prime numbers.

    Example:
        ```
        n_primes_array = generate_n_primes(10, 50)
        print(n_primes_array)
        ```
    """
    return _PP.generate_n_primes(int(n), int(start))

def get_nth_prime(n: Union[int, float], start: int = 0) -> int:
    """
    Get the n^th prime number.

    Args:
        n (int): The position of the prime number to retrieve.
        start (int, optional): The starting value to search for the prime number.

    Returns:
        int: The n^th prime number.

    Example:
        ```
        nth_prime = get_nth_prime(10, 50)
        print(nth_prime)
        ```
    """
    return _PP.get_nth_prime(int(n), int(start))

def count_primes(start: Union[int, float], stop: Union[int, float]) -> int:
    """
    Count the number of primes in the given range.

    Args:
        start (int): The starting value of the range.
        stop (int): The ending value of the range.

    Returns:
        int: The count of primes in the specified range.

    Example:
        ```
        primes_count = count_primes(10, 50)
        print(primes_count)
        ```
    """
    return _PP.count_primes(int(start), int(stop))

def count_twins(start: Union[int, float], stop: Union[int, float]) -> int:
    """
    Count the number of twin primes in the given range.

    Args:
        start (int): The starting value of the range.
        stop (int): The ending value of the range.

    Returns:
        int: The count of twin primes in the specified range.

    Example:
        ```
        twins_count = count_twins(10, 50)
        print(twins_count)
        ```
    """
    return _PP.count_twins(int(start), int(stop))

# Add wrapper functions for other count functions similarly...

def get_max_stop() -> int:
    """
    Get the maximum allowed stop value for prime-related functions.

    Returns:
        int: The maximum allowed stop value.

    Example:
        ```
        max_stop = get_max_stop()
        print(max_stop)
        ```
    """
    return _PP.get_max_stop()

def is_prime(number: Union[int, float]) -> bool:
    """
    Check if a given number is a prime number.

    Args:
        number (int): The number to check for primality.

    Returns:
        bool: True if the number is prime, False otherwise.
    """
    return _PP.is_prime(int(number))
