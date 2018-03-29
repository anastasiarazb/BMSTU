#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import turtle

class Square(turtle.Turtle): ##Отрисовка квадрата с верхнего левого угла
    def __init__(self, side=4, x=0, y=0, filled=False):
        self.side = side;
        self.x = x
        self.y = y
        self.filled = filled

    def l_up_corner(self) : return self.x, self.y
    def l_down_corner(self) : return self.x, self.y - self.side
    def r_up_corner(self) : return self.x + self.side, self.y
    def r_down_corner(self):
        return self.x + self.side, self.y - self.side
    
    def draw(self, filled=False, color = "black"):
        turtle.up()
        turtle.setpos(self.x, self.y)
        turtle.down()
        if filled: turtle.begin_fill()
        for i in range(4) :
            turtle.fd(self.side)
            turtle.right(90)
        if filled: turtle.end_fill()

colors = ["red", "yellow", "blue", "green", "brown", "gray"]
def paint_row(number = 6, side = 5, x = 0, y = 0, filled = False) :
    for i in range(number):
        sq = Square(side, x, y)
        if filled: turtle.begin_fill()
        turtle.color(colors[i])
        sq.draw()
        if filled: turtle.end_fill()
        x, y = sq.r_up_corner()

def four_squares(a = 50, filled = False, x = 0, y = 0):
    b = 4*a
    for i in range(4):
        sq = Square(b, x, y)
        if filled: turtle.begin_fill()
        turtle.color(colors[i])
        sq.draw()
        if filled: turtle.end_fill()
        x, y = sq.l_up_corner()
        x += a/2
        y -= a/2
        b -= a
        

def blue_circle(x=-100, y=-200) :
    turtle.up()
    turtle.setpos(x, y)
    turtle.down()
    turtle.color("blue")
    turtle.begin_fill()
    turtle.circle(50)
    turtle.end_fill()

blue_circle(-200, -200)
paint_row(x = -100, y = -50, side = 50)
paint_row(x = -200, y = 20, side = 50, filled = True)
four_squares(50, x = -200, y = 250)
four_squares(x = 50, y = 250, a = 50, filled=True)

##turtle.begin_poly()
turtle.hideturtle()
##turtle.color("red")
##sq = Square(100)
##sq.draw()
##x, y = sq.r_up_corner()
##sq = Square(100, x, y)
#sq.draw()

#turtle.goto(100, 0)
##turtle.end_poly()


##Используя модуль turtle, реализуйте:
##
##Рисование окружности синего цвета с радиусом 50 пикселей.
##
##Рисование 6 квадратов разного цвета в один ряд (не закрашивать).
#Здесь и далее рисование квадрата должно быть оформлено в виде функции
##или метода класса, унаследованного от класса Turtle.
##
##Рисование 4 квадрата со сторонами a, 2a, 3a, 4a (a — произвольное).
##
##То же, квадраты закрасить.
##
##Рисование спирали из ломаных линий из начала координат.

