#!/usr/bin/python
# coding=utf-8
#
# markov.py
#
# Author: David Rottenfusser
# Date: 5 Apr 2014
#
# Builds a hidden Markov model of Lorem Ipsum, then executes the model to
# generate a pseudorandom sequence of characters in the same style.
#
# The model has one-letter memory, and chooses the next letter based on the
# frequency distribution in the source text.  The program stops when the
# model generates a zero.
#
# 5 Apr 2014: Translated C++ to Python, and removed command line parameter
#   support.
#

import sys
import random

_lorem = "Lorem ipsum dolor sit amet, consectetur adipisicing elit, sed do \
eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim \
veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea \
commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit \
esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat \
non proident, sunt in culpa qui officia deserunt mollit anim id est laborum."

def _next(freq):
    sum = 0
    for item in freq.itervalues(): sum += item
    if sum == 0: return 0

    r = random.randint(0, sum - 1)
    for key in iter(freq):
        r -= freq[key]
        if r < 0: return key

    return 0


def markov(src = _lorem):
    if len(src) == 0: src = _lorem

    last = 0
    t = dict()
    for c in src:
        if c == 0: continue

        if last not in t:    t[last] = dict()
        if c not in t[last]: t[last][c] = 0
        t[last][c] += 1
        last = c

    if last not in t:    t[last] = dict()
    if 0 not in t[last]: t[last][0] = 0
    t[last][0] += 1

    random.seed()
    last = 0
    limit = 100000
    out = ''
    while limit > 0:
        c = _next(t[last])
        if (c == 0): break
        out += c
        last = c
        limit = limit - 1

    print out

if __name__ == "__main__":
    markov()
