import sys

def is_python_3():
    print(sys.version_info[0])
    return sys.version_info[0] == 3

def str_slice(string,idx,st_idx = -1):
    res = ""
    for i in range(0,len(string)-1):
        if(i <= st_idx and i >= idx):
            res += string[i]
    return res