import os
import sys
import PyUtils
import distutils.spawn


if(PyUtils.is_python_3()):
    from shutil import which

if(PyUtils.is_python_3()):
    cmake_path = which("cmake")
else:
    cmake_path = distutils.spawn.find_executable("cmake")
if cmake_path == None:
    print("CMake executable not found in path... Exiting...")
    exit(1)

args = sys.argv
args.pop(0)
# print(args)

defines = []
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
