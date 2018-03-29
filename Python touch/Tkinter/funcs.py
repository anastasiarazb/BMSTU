from math import *
from tkinter import *
##root=Tk()
##canvas=Canvas(root,width=600,height=600, bg = "white")
##canvas.pack()
##Button(root, text='Quit', command=root.destroy).pack(side=BOTTOM, anchor=SE)
##
##offset = 5
##dx = 40
##xs = offset
##xf = xs + dx
##yf = ys = offset
##zs = zf = 460
##
##for i in range(1,21):
##    canvas.create_line(xs, ys, xf, yf, width=2,fill="blue")
##    canvas.create_line(xs, zs, xf, zf, width=2,fill="green")
##    canvas.create_line(xs, ys, xs+1, ys, fill='white')
##    canvas.create_line(xs, zs, xs+1, zs, fill='red')
##    xs = xf
##    ys = yf
##    zs = zf
##    xf = xf + dx
##    yf = i*i + offset
##    zf = 460 - i*i
##
##canvas.create_line(300, 300, 300, 300, fill='red')
##
##root.mainloop()

class Graph:
        
    def __init__(self, x_min, x_max, width, height, func=lambda x: x, limits = False):
        self.x_min = x_min
        self.x_max = x_max
        self.width = width
        self.height = height
        self.f = func
        self.kx = width/(x_max-x_min) #число пикселей в единичном отрезке по x
        
        self.make_arg_list()
        #print(list(self.arg_list))
        self.res_list = list(map(self.safe_func, self.arg_list))
        comparator = lambda x: x if x != None else 0
        self.y_min = min(self.res_list, key = comparator)
        self.y_max = max(self.res_list, key = comparator)
##        if self.y_min >= 0 :
##            self.y_min = -self.y_max / 10
##        if self.y_max <= 0 :
##            self.y_max = -self.y_min / 1010
##        if limits:
##            if self.y_min < -100 : self.y_min = -100
##            if self.y_max > 100 : self.y_max = 100
        
        #число пикселей в единичном отрезке по y
        self.ky = (height-30)/((self.y_max - self.y_min))
        #self.points = zip(self.arg_list, self.arg_list, self.res_list, self.res_list)

    def __str__(self):
        string = str(self.width) + ' ' + str(self.height) + '\n'
        string += str(self.x_min) + ' ' + str(self.x_max) + '|'
        string += str(self.y_min) + ' ' + str(self.y_max) + '\n'
        string += "kx/ky " + str(self.kx) + ' ' + str(self.ky) + '\n'
        string += "display corners " + str(self.screen_x_y(self.x_min, self.y_min))
        string += " " + str(self.screen_x_y(self.x_max, self.y_max)) + "\n"
        print(self.res_list)
        return string
        

    def safe_func(self, x): #Если функция не определена в точке
        try : return self.f(x)
        except: return None
        
    def make_arg_list(self): #Не получается использовать короткий range
        self.arg_list = []
        x = self.x_min
        i = 0
        delta = 1/self.kx
        while x <= self.x_max:
            self.arg_list.append(x)
            x = self.x_min + delta * i
            i += 1
    def screen_x(self, x):
        return self.kx * (x - self.x_min)
    def screen_y(self, y):
        if y == None : return None
        return self.height - self.ky * (y - self.y_min) - 21
    def screen_x_y(self, x, y):
        return self.screen_x(x), self.screen_y(y)
    def make_dots(self):
        all_x = list(map(self.screen_x, self.arg_list))
        all_y = list(map(self.screen_y, self.res_list))
        self.coords = list(zip(all_x, all_y))
        
        self.coords = list(filter(lambda x : x[1] != None, self.coords))
        ##Провести линии между соседними точками, для которых определено значение
        self.lines = []
        for i in range(len(self.coords)-1):
            if self.coords[i][1] != None and self.coords[i+1][1] != None:
                self.lines.append(self.coords[i] + self.coords[i+1])
       # print(self.coords)
        
