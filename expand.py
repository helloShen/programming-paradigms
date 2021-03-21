import sys
import random

grammar = {'<start>':[['This ', '<object>', ' is here']],
            '<object>':[['computer'], ['car'], ['assignment']]}

def expand(symbol):
    if symbol.startswith('<'):
        definitions = grammar[symbol]
        expansion = random.choice(definitions)
        map(expand, expansion)
    else:
        sys.stdout.write(symbol)

random.seed()
expand('<start>')