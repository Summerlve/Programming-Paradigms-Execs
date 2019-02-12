#-*- coding:utf-8 -*-

class People:
    num = 1000
    def __init__(self, name, age):
        self.name = name
        self.age = age
    
    def getName(self):
        return self.name

i = People("zhang", 200)
print "i.num: ", i.num
print "People.num", People.num