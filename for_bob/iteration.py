from sympy import *
from math import ceil

class Solve: #класс, решающий уравнение методом сжимающих отображений
    def __init__(self, func, x, step=0.01, interval=(0, 4), eps=10**-5):#конструктор класса
        self.x = x #переменная для вычислений
        self.func = func
        self.diff_func = diff(func, x)#прозводная функции для нахождения параметра сжатия 

        self.step = step#шаг для поиска отрезков дельта

        self.interval = interval#область поиска
        self.eps = eps#точность вычислений для корня

    def calc_delta(self):#метод поиска отрезков дельта в заданом диапазоне
        intervals = []
        
        search_end = self.step + self.interval[0]

        while search_end <= self.interval[1]:
            delta_l = self.func.subs(x, search_end - self.step)
            delta_r = self.func.subs(x, search_end)

            if(sign(delta_l) != sign(delta_r)):
                intervals.append((search_end - self.step, search_end))
            
            search_end += self.step

        return intervals


    def calc_diff_max_and_min(self, left, right):
        
        maxx = self.diff_func.subs(x, right)
        minn = self.diff_func.subs(x, left)
        y = float(left)
        step = self.step
        
        while y <= left:
            sol = self.diff_func.subs(x, y)
            maxx = max(maxx, sol)
            minn = min(minn, sol)
            y += step
        print(maxx)
        print(minn)
        return (maxx, minn)

    def cout_iteration(self, alpha, phi, interval):#метод для подсчёта количества итераций
        x0 = phi.subs(self.x, interval[0])
        x1 = phi.subs(self.x, x0)
        
        return log(self.eps * (1 - alpha) / abs(x1 - x0), alpha) #выражение параметра сжатия альфа из оценки точности
    
    def solve_equation(self):
        
        print("function: ")
        print(f"f(x) = {self.func}\n")
        
        print("function derivative: ")
        print(f"f'(x) = {self.diff_func}\n")

        deltas = self.calc_delta()
        if(len(deltas) == 0):
            print("no roots on this interval")
            return

        print("roots local:")
        for delta in deltas:
            print(delta)

        print("\nmain solution:\n")

        for a, b in deltas: #перебор всех отрезков, на которых локализованы корни
            print(f"delta = [{a}, {b}]:")
            max_min = self.calc_diff_max_and_min(a, b)
            
            lambda_par = 2 / (max_min[0] +  max_min[1]) #коффициент лямбда  для сжимающей функции
            phi = self.x - lambda_par * self.func #сжимающее отображение

            alpha = abs(((max_min[0] - max_min[1])) / (max_min[0] + max_min[1])) #коэффициент альфа для поиска числа итераций
            iterations = ceil(self.cout_iteration(alpha, phi, (a, b))) + 2

            print(f"number iterations= {iterations}")

            x0 = a
            print(f"x0={x0}")
            for i in range(1, iterations + 2):
                x0 = phi.subs(x, x0)
                print(f"x{i} = phi(x{i-1}) = {x0}")
            
            print()
            print()


x = Symbol("x")

f = 5*x + log(3*x + 5, 10) -8

A = Solve(f, x)

A.solve_equation()


