
#Символы, которые не являются частью слов
punctuation = ",\"\' ?!@#$%^&*(){}[]\\|/:;<>~.-_+="

class Token:
    def __init__(self, line=-1, col=-1, word="empty token", is_punctum=False) :
        self.line = line
        self.col = col,
        self.word = word
        self.is_punctum = is_punctum
        
    def __str__(self) :
        return "{} {} {}".format(self.line, self.col, self.word)
    
    def in_dictionary(self, dictionary) :
        return self.is_punctum or (self.word in dictionary)

def incorrect(word, dict) :
    ok = word.isnumeric() or len(word) == 0;
    return not ok and (word.lower() not in dict)

def scanner(dictionary, text) :
    tokens = []
    text = text.splitlines()
    line_number = 1
    for line in text:
        column = 0;
        word = ""
        for char in line :
            column += 1
            if char.isspace() :
                if incorrect(word, dictionary):
                    print(line_number, column - len(word), word)
                word = ''
            elif char not in punctuation :
                word += char;
        line_number += 1
##    print(line_number)

if __name__ == '__main__' :
    dict = {'x', 'y', 'z', 'delta', 'd`arc', 'gamma', 'alpha', 'beta'}
    x = Token(0, 0, "asd")
    print(x.in_dictionary(dict))
    text = "\n\n sdfv   asd vxcz Asdf d`ARC  fdsa \n cxz"
    print(text.splitlines());
    scanner(dict, text)

##Все приводится к нижнему регистру, имена собственные, написанные с маленькой
#буквы, ошибкой не считаются
