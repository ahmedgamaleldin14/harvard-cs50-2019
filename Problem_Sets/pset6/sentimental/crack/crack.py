import sys
import string
from crypt import crypt
from itertools import product


def main():
    # check if more or less arguments are given
    if len(sys.argv) != 2:
        print("Usage: python crack.py hash")
        # stop the program
        sys.exit(1)

    hashPass = sys.argv[1]
    salt = hashPass[0:2]

    letters = string.ascii_letters
    # pre-declaration of methods reduces overhead
    join = ''.join

    for i in range(1, 6):
        # use built-in itertools to reduce the time
        for key in product(letters, repeat=i):
            key = join(key)
            # use crypt built-in function to figure out the hash key
            if (crypt(key, salt) == hashPass):
                print(key)
                sys.exit(0)

    print("Password can't be cracked")


if __name__ == "__main__":
    main()