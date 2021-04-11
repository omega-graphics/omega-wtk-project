import sys

def is_python_3():
    print(sys.version_info[0])
    return sys.version_info[0] == 3

def str_slice(string:str,idx,st_idx = 0):
    res = ""
    for i in range(0,len(string)):
        c = string[i]
        if i > idx or i < st_idx:
            res += c
    return res