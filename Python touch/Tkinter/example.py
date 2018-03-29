from tkinter import *
 
##class But_print:
##     def __init__(self):
##          self.but = Button(root)
##          self.but["text"] = "Печать"
##          self.but.bind("<Button-1>",self.printer)
##          self.but.pack()
##     def printer(self,event):
##          print ("Как всегда очередной 'Hello World!'")
## 
##root = Tk()
##root.
##obj = But_print()
##root.mainloop()

x = list(map(lambda x : x, [1, 2, 3]))
print(x)
y = list(map(lambda x : None if x == 2 else x, [1, 2, 3]))
print(y)
z = list(zip(x, y))
print(list(z))
print(z[1][-1])
z2 = list(filter(lambda x: x[1] != None, z))
print(list(z2))
a = [1,2]
b = [3,4]
print(list(zip(a,b)))

z.append((-1, 2))
lines = []
for i in range(len(z)-1):
        if z[i][1] != None and z[i+1][1] != None:
            lines.append(z[i] + z[i+1])
print(lines)

str = 'x**2'

y = lambda x: eval(str)
print(y(2))
