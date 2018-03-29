#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import turtle

class Spiral(turtle.Turtle):
    def __init__(self, angle=45, delta=10, steps=20):
        self.delta = delta
        self.angle = angle
        self.steps = steps
        
    def draw(this):
        turtle.reset()
        turtle.up()
        turtle.home()
        turtle.down()
        length = this.delta
        for counter in range(this.steps) :
            turtle.fd(length)
            turtle.left(this.angle)
            length += this.delta

            
if __name__ == '__main__' :
    turtle.hideturtle()
    turtle.color("red")
    Spiral().draw()
    line = Spiral(50, 2, 250)
    line.draw()

##Используя модуль turtle, реализуйте:
##
##Рисование окружности синего цвета с радиусом 50 пикселей.
##
##Рисование 6 квадратов разного цвета в один ряд (не закрашивать). Здесь и далее рисование квадрата должно быть оформлено в виде функции или метода класса, унаследованного от класса Turtle.
##
##Рисование 4 квадрата со сторонами a, 2a, 3a, 4a (a — произвольное).
##
##То же, квадраты закрасить.
##
##Рисование спирали из ломаных линий из начала координат.

##turtle.begin_poly(); turtle.end_poly()

