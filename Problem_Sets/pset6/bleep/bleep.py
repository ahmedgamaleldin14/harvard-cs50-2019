from cs50 import get_string
from sys import argv, exit


def main():
    if len(argv) != 2:
        print("Usage: python bleep.py dictionary")
        exit(1)

    message = get_string("What message would you like to censor?\n")
    # put the message words into a list
    message_list = message.split()

    file = open(argv[1])
    file_words = list()

    # save the file words into a list but remove "\n" using strip
    for word in file:
        file_words.append(word.strip())

    for word in message_list:
        if word.lower() in file_words:
            message = message.replace(word, '*' * len(word))

    print(message)

    file.close()


if __name__ == "__main__":
    main()
