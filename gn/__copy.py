import sys
import os
import PyUtils

if(PyUtils.is_python_3()):
    import shutil
else:
    import os

args = sys.argv
args.pop(0)

src = None 
dest = None

for arg in args:
    if arg == "--src":
        idx = args.index(arg)
        src = args[idx + 1]
    elif arg == "--dest":
        idx = args.index(arg)
        dest = args[idx + 1]

if(sys.api_version > 2.9):
    if(os.path.isfile(src) and not os.path.exists(dest)):
        shutil.copyfile(src,dest)
    elif(os.path.isdir(src) and not os.path.exists(dest)):
        shutil.copytree(src,dest,True)
else:
    os.system("cp " + src + " " + dest)