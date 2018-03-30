import re

def make_dictionary(dictionary) :
    #Символы, которые не являются частью слов
    punctuation = ",\"\' ?!@#$%^&*(){}[]\\|/:;<>~.-_+="
    try:
        file = open(dictionary)
        str = file.read()
        file.close()
    except IOError:
        print("Can't open file", dictionary)
        return None
    words = str.split()
    i = 0
    #Убрать в кажом слове пунктуацию и привести к нижнему регистру
    while i < len(words) :
        words[i] = words[i].strip(punctuation).lower()
        i += 1
##    print(sorted(words))
    return set(words)

if __name__  == '__main__' :
    dict = make_dictionary("dictionary")
    print(dict)

