import re


def tryint(s):
    """Try to convert input s into a int, else, return native type"""
    try:
        return int(s)
    except:
        return s


def alphanum_key(s):
    """ Turn a string into a list of string and number chunks.
        "z23a" -> ["z", 23, "a"]
    """
    return [tryint(c) for c in re.split('([0-9]+)', s)]


def naturalsort(l):
    """ Sort the given list in the way that humans expect.
    """
    l.sort(key=alphanum_key)
