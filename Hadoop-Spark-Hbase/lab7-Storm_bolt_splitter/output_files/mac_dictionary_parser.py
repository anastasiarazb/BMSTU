import re

#В Python стандартная замена 0-9 на \d
mac_regex = "^([\da-fA-F]{2}[:-]){5}[\da-fA-F]{2}$"
pattern = re.compile(mac_regex)

with open('test/mac_dictionary.txt', 'r') as content_file:
    content = content_file.read()

words = content.split()

outp = open('output.xml','w')

outp.write('<?xml version="1.1" encoding="UTF-8" ?>\n')
outp.write('<txt>\n')

counter = 0
for s in words:
    if pattern.match(s) != None:
        outp.write('\t<inx>'+ s + '</inx>\n')
        counter += 1
        
outp.write('</txt>\n')

outp.close()

print(counter, "matches found.")
