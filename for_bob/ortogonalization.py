from sympy import *

SYMBOL = Symbol("t")#глобальная константа для хранения независимой переменной пространства C

class Ortonorm: #класс, содержащий в себе алгоритм построения нормированной системы для любого Евклидова пространства
    def __init__(self, input_system, n):#конструктор класса, принимающий список векторов пространства и количество векторов нормированной системы, которые нужно сгенерировать
        self.vlist = input_system
        self.len = n
        
        self.phi_sys = []
    
    
    def generate_h(self, n):#вычисление функции h для fn 
        bij = []#коэффициенты b из алгоритма нахождения функций h
        for i in range(n):#вычисление коэффициентов b
            b = self.scalar_mul(self.vlist[n], self.phi_sys[i])
            bij.append(b)
        h = self.vlist[n]

        for i in range(n):
            h -= self.phi_sys[i] * bij[i]

        return h

    def norm_h_to_phi(self, h):#нормирмализация h
        return h / sqrt(self.scalar_mul(h, h))


    def generate_norm_sys(self):#вычисление нормированной системы
        
        for i in range(0, self.len):
            h = self.generate_h(i)

            self.phi_sys.append(self.norm_h_to_phi(h))
            
            print(f"phi{i+1} = {self.phi_sys[i]}")


class Ortonorm_C(Ortonorm):#наследованный класс от Ortonorm с переопределённым скалярным произведением для пространстваC 
    def scalar_mul(self, x, y):#скалярное произведение пространства C
        return integrate(x*y, [SYMBOL, 0, 1])
        

class Ortonorm_l2(Ortonorm):#наследованный класс от Ortonorm с переопределённым скалярным произведением для пространства l2            
    
    def scalar_mul(self, x, y):#скалярное произведение пространства l2
        accamulator = 0 
        for i in range(len(x)):
            accamulator += x[i] * y[i]
        return accamulator



vector_sys = [Array([1,-1,0,0,0,0]), Array([0,1,-1,0,0,0]), Array([0,0,1,-1,0,0]), Array([0,0,0,1,-1,0])]#ввод входной системы векторов в l2
new_vector_sys_generator = Ortonorm_l2(vector_sys, len(vector_sys))#объект класса, который строит нормированную систему
new_vector_sys_generator.generate_norm_sys()#построение нормированной системы и вывод её на экран

print()

func_sys = [1, SYMBOL, SYMBOL**2, SYMBOL**3, SYMBOL**4]#ввод входной системы функций в C
new_func_sys_generator = Ortonorm_C(func_sys, len(func_sys))#объект класса, который строит нормированную систему
new_func_sys_generator.generate_norm_sys()#построение нормированной системы
