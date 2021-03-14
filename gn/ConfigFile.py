import os
import sys
from shutil import which

cmake_path = which("cmake")
if cmake_path == None:
    print("CMake executable not found in path... Exiting...")
    exit(1)

args = sys.argv
args.pop(0)
# print(args)

defines:"list[str]" = []
input = None 
output = None

for arg in args:
    if arg.startswith("-D"):
        idx = args.index(arg)
        defines.append(args[idx])
    elif arg == "--input" or arg == "-I":
        idx = args.index(arg)
        input = args[idx + 1]
    elif arg == "--output" or arg == "-O":
        idx = args.index(arg)
        output = args[idx + 1]

cwd = os.getcwd()
if input != None and output != None:
    print("Running cmake with command:" + "cmake " + "-DSOURCE=" + input + " -DDEST=" + output + " " + " ".join(defines) + " -P " + cwd + "/config_file.cmake")
    os.system("cmake " + "-DSOURCE=" + input + " -DDEST=" + output + " " + " ".join(defines) + " -P " +  "../gn/config_file.cmake")
