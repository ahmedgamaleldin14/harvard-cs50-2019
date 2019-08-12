import sys
from nltk.tokenize import sent_tokenize
import itertools

def lines(a, b):
    """Return lines in both a and b"""

    # pre-allocate lists
    a_list = []
    b_list = []
    similar_lines = []

    # append the lines of the file in a list
    for line in a.splitlines():
        a_list.append(line.strip())

    for line in b.splitlines():
        b_list.append(line.strip())

    # check if they have lines in common
    for line in a_list:
        if line in b_list:
            similar_lines.append(line)

    # remove duplicates using set
    similar_lines = list(set(similar_lines))
    print(similar_lines)

    return similar_lines


def sentences(a, b):
    """Return sentences in both a and b"""

    # use built-in library to divide the string into sentences based on the punctuation
    a_list = sent_tokenize(a.replace("\n", ""))
    b_list = sent_tokenize(b.replace("\n", ""))
    similar_sentences = []

    [similar_sentences.append(sentence) for sentence in b_list if sentence in a_list]

    similiar_sentences = list(set(similar_sentences))

    return similar_sentences


def substrings(a, b, n):
    """Return substrings of length n in both a and b"""

    a_list = []
    b_list = []
    similar_substrings = []

    # iterate over the string and slice it into n characters
    a_list = slicer(a, n)
    b_list = slicer(b, n)

    [similar_substrings.append(sub) for sub in a_list if sub in b_list]
    similar_substrings = list(set(similar_substrings))

    return similar_substrings

def slicer(file_str, n):
    lst = []
    for i in range(len(file_str) - n + 1):
        lst.append(file_str[i:i+n])

    return lst

