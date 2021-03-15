import sys
import shutil


args = sys.argv
args.pop(0)

src:str 
dest:str

for arg in args:
    if arg == "--src":
        idx = args.index(arg)
        src = args[idx + 1]
    elif arg == "--dest":
        idx = args.index(arg)
        dest = args[idx + 1]

shutil.copyfile(src,dest)