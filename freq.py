#Exercise 1
#Christopher Pontikes 1499276

import string
import sys
import argparse

parser = argparse.ArgumentParser(
    description='Sort a text file.',
    formatter_class=argparse.RawTextHelpFormatter,
    )

parser.add_argument("--sort",
    help="""Sorting the text
    byfreq(default): by frequency
    byword: alphabetically""",
    choices=['byfreq','byword'],
    default='byfreq',
    dest='sort')

parser.add_argument("infile",
    help="file to be sorted, stdin if omitted",
    nargs="?",
    type=argparse.FileType('r'),
    default=sys.stdin)


parser.add_argument("--ignore-case",
    help="ignore upper/lower case when doing all actions.",
    action="store_true",
    dest="case")

parser.add_argument("--remove-punct",
    help="remove all punctuation characters in a word preserving only the alphanumeric characters",
    action="store_true",
    dest="punct",)


args = parser.parse_args()
dictionary = {}
numwords = 0
for line in args.infile:

    text = line

    if args.punct:
        #removes punctuation along with import string
        for x in string.punctuation:
            text = text.replace(x,"")

    if args.case:
        text = text.lower() #to ignore upper and lower case


    words = text.split()

    for x in words:
        numwords+=1
          #checks if word is already in the dictionary
            #if so, add one to the counter
        if x in dictionary:
            dictionary[x] += 1


        #otherwise, make the word the key of the dictionary and set its value to 1
        else:
            #dictionary[x] = x
            dictionary[x] = 1


count = []
frequency = []

#get the keys seperate
keys = dictionary.keys()
words_compressed = []

#go through all the keys
for x in keys:
    #add the keys from the dictionary into a new list
    words_compressed.append(x)
    #add the counters of each word to a new list
    count.append(dictionary[x])

for x in count:
    #calculate the frequency
    num = round(x/numwords,2)
    #add the freqeuncy of each word to a new list
    frequency.append(num)


#zip all the new lists into a list of tuples
everything = list(zip(words_compressed, count, frequency))

#sorts alphabetically this one is needed
everything.sort()

#sort based on frequency
frequency_sort = sorted(everything, key = lambda x: x[2])
#sorts reverse frequency this one is needed
f = list(reversed(frequency_sort))

if args.sort == "byfreq":
    for x in f: #x in everything for alphabetical, x in f for frequency
        print(x)

elif args.sort == "byword":
    for x in everything: #x in everything for alphabetical, x in f for frequency
        print(x)

print("number of words: ", numwords)
