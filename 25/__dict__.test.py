#-*- coding:utf-8 -*-

class Test:
    def __init__(self):
        self.a = 17

t = Test()
t.__dict__["b"] = "hello"
t.__dict__["c"] = False

print t.b, t.c
print t.__dict__

t.name = "zhang"
print t.name