from tkinter import *
from tkinter import font  # интерпретатор видит все модули из tkinter, кроме этого
from tkinter import messagebox
from funcs import *
from math import *
root = Tk()
root.title("Plot")
root.geometry('800x400')

font_x_y = font.Font(family="Helvetica", size=10, slant=font.ITALIC)
def draw_graph():
    try:
        x_min = float(field2.get())
        x_max = float(field3.get())
    except:
        messagebox.showerror("Wrong Bounds",
                             "Неверные границы построения!")
        return
    width = graph.winfo_reqwidth()
    height = graph.winfo_reqheight()
    g = Graph(0, 1, 1, 1)
    try :
        func = lambda x: eval(formula.get())
        g = Graph(x_min, x_max, width, height, func, limits = False)
    except:
        messagebox.showerror("Wrong Formula",
                             "Проверьте корректность введения формулы!")
    graph.delete(ALL)
    graph.create_line(g.screen_x_y(x_min, 0), g.screen_x_y(x_max, 0))
    graph.create_line(g.screen_x_y(0, g.y_min), g.screen_x_y(0, g.y_max))
    graph.create_text(g.screen_x(x_max) - 25, g.screen_y(0) + 15, text = "x",
                      font = font_x_y)
    graph.create_text(g.screen_x(0) - 15, g.screen_y(g.y_max) + 15, text = "y",
                      font = font_x_y)
    graph.create_text(g.screen_x(0) - 10, g.screen_y(0) + 10, text = "O",
                      font = font_x_y)
    graph.create_polygon(g.screen_x(0)-3, g.screen_y(g.y_max)+7,
                         g.screen_x_y(0, g.y_max),
                         g.screen_x(0)+3, g.screen_y(g.y_max)+7,
                         g.screen_x(0), g.screen_y(g.y_max)+5)
    graph.create_polygon(g.screen_x(x_max)-20, g.screen_y(0)-3,
                         g.screen_x(x_max) - 13, g.screen_y(0), 
                         g.screen_x(x_max)-20, g.screen_y(0)+3,
                         g.screen_x(x_max) - 18, g.screen_y(0))
    g.make_dots()
    for line in g.lines:
        graph.create_line(line, width = 2)
    ##for point in g.coords:
        ##graph.create_line(point, point)
        
   # print(g)
    
    #print(g.screen_x_y(x_min, 0), g.screen_x_y(x_max, 0), g.screen_x_y(0, g.y_min), g.screen_x_y(0, g.y_max))
    
def print_axes():
    x0, y0 = graph.winfo_x(), graph.winfo_y()
    

def printf(event):
    print(formula.get())
  #  formula.set('sda')
    #new_geom = str(root.winfo_x) +'x'+ str(root.winfo_y)
    #frame1.geometry(new_geom)


def canvas_resize(event):
    width = event.width
    height = event.height
    #width = frame2.winfo_width()
    #height = frame2.winfo_height()
    graph.config(width = width, height = height)
    draw_graph()
    #print(width, height)
    
frame1=Frame(root,width=100,heigh=100, bd=5)
frame2=Frame(root, width = 200, heigh=75, bd=5, bg='white')
frame1.pack(side='bottom', fill='x')
frame2.pack(side='bottom', ipadx=50)
frame2.bind("<Configure>", canvas_resize)
graph = Canvas(frame2, bg = "white")
graph.pack()

label1 = Label(frame1 ,text='f(x) = ')
label2 = Label(frame1, text=' x min = ')
label3 = Label(frame1, text=' x max = ')
button = Button(frame1, text = 'Plot!')

formula = StringVar()
x_min = StringVar()
x_max = StringVar()
formula.set('x')
x_min.set("-1")
x_max.set("1")

field1 = Entry(frame1, textvariable=formula)
field2 = Entry(frame1, textvariable=x_min, width=15)
field3 = Entry(frame1, textvariable=x_max, width=15)



button.bind('<Button-1>', lambda event : draw_graph())
root.bind('<Return>', lambda event : draw_graph()) #ord(event.char) == 13

label1.pack(side='left')
field1.pack(side = 'left', fill='x', expand=True)
label2.pack(side = 'left')
field2.pack(side = 'left')
label3.pack(side = 'left')
field3.pack(side = 'left')
button.pack(side = 'right', padx=10)

draw_graph()

root.mainloop() 

##Entry: получать значение можно и не через привязанный объект StringVar()
##, а через объект Entry, преимущество StringVar() в возможности изменять поле
## через метод set()

#Для связывания события нажатия клавиши на клавиатуре "<Key>" с
#функцией-обработчиком используйте метод класса виджета bind.
#Нажатие клавиши Enter определяйте по коду символа, например, так:
#ord(event.char) == 13 //Для одного Enter можно использовать только '<Return>'
#event — событие, переданное в качестве аргумента функции-обработчику).

#label1=Label(frame1 ,text='f(x) = ', font='arial 10')
