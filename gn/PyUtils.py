

def str_slice(string:str,idx:int) -> str:
    res = ""
    for i in range(0,string.count()-1):
        if(i > idx):
            res += string[i]
    return res