from cs50 import get_string
import sys

# make sure that the user input is only digits
def main():
    while True:
        cardNumber = get_string("Number: ")
        if (cardNumber.isdigit()):
            break

    # return card type, if not valid, type INVALID
    cardType = check_structure(cardNumber)

    #reverse the list
    cardNumber = [int(i) for i in reversed(cardNumber)]
    # return the first addition done on the digits
    first_addition = add(cardNumber)

    # second addition
    addition = sum(first_addition) + sum(cardNumber[0::2])

    if (addition % 10 != 0):
        print("INVALID")
    else:
        print(cardType)


# check the keys of every paypal
def check_structure(number):
    number_check = int(number[0:2])
    if (int(number[0]) == 4 and (len(number) == 13 or len(number) == 16)):
        return "VISA"
    elif ((number_check >= 51 or number_check <= 55) and len(number) == 16):
        return "MASTERCARD"
    elif((number_check == 34 or number_check == 37) and len(number) == 15):
        return "AMEX"
    else:
        print("INVALID")
        sys.exit()


# calculate the first addition
def add(number):
    number = [2 * i for i in number[1::2]]
    addition = []
    for i in range(len(number)):
        if number[i] > 9:
            addition.extend([int(number[i]/10), number[i]%10])
        else:
            addition.append(number[i])

    return addition


if __name__ == "__main__":
    main()