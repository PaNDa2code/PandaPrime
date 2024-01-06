import numpy as np
from os.path import join as join_path

def PRIMES_LIST():
    path_to_primes = join_path("/".join(__file__.split("/")[:-1]), "primes.npy")
    primes = np.memmap(path_to_primes, dtype="uint32", mode="r", shape=78498)
    return primes