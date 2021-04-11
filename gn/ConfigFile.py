
import os
import sys
import PyUtils
import re
# import out
import shutil

class ConfigFileProcessor:
    
    code:str
    defines:"dict[str,str]"
    def __init__(self,code:str,defines:"dict[str,str]"):
        self.code = code
        self.defines = defines
    def process(self):
        for d in self.defines:
            self.code = re.sub("@" + d+ "@",self.defines[d],self.code,flags=re.MULTILINE)
            



def main():

    args = sys.argv
    args.pop(0)
    # print(args)

    defines:"dict[str,str]" = {}
    input = None 
    output = None

    for arg in args:
        if arg.startswith("-D"):
            idx = args.index(arg)
            s = args[idx]
            s = PyUtils.str_slice(s,1,0)
            res = s.split("=")
            print(res)
            if len(res) > 1:
                defines[res[0]] = res[1]
            else:
                defines[res[0]] = ""
        elif arg == "--input" or arg == "-I":
            idx = args.index(arg)
            input = args[idx + 1]
        elif arg == "--output" or arg == "-O":
            idx = args.index(arg)
            output = args[idx + 1]

    cwd = os.getcwd()
    if input != None and output != None:
        fstream = open(input,"r")
        config = ConfigFileProcessor(code=fstream.read(),defines=defines)
        config.process()
        f = open(output,"w")
        f.write(config.code)
        print("Sucessfully Configured File")

if __name__ == "__main__":
    main()