from cs50 import get_int

def main():
    while True:
        hashNumber = get_int("Height: ");
        if (1 <= hashNumber <= 8):
            break

    for i in range(hashNumber):
        print(" " * (hashNumber-(i+1)), end="")
        print("#" * (i+1), end="")
        print("  ", end="")
        print("#" * (i+1))

if __name__ == "__main__":
    main()