#-*- coding:utf-8 -*-
from random import choice, seed
import sys

grammar = {
    "<start>": [["This", "<object>", "is here"]],
    "<object>": [
        ["computer"],
        ["car"],
        ["assignment"]
    ]
}

def expand(symbol):
    if symbol.startswith("<"):
        definiations = grammar[symbol]
        expansions = choice(definiations)
        map(expand, expansions)
    else:
        sys.stdout.write(symbol + " ")

seed()
expand("<start>")