#
# A simple script for compiling shaders
#

import os
import sys
import argparse

parser = argparse.ArgumentParser(description="A python script used for compiling shader files")


parser.add_argument("--metallib",dest="metallib",action='store_const',const=True,default=False)
parser.add_argument("-O",type=str)
parser.add_argument("files",metavar='files',nargs="+",type=str)
args = parser.parse_args()
if(args.files == None):
    print("No input Files.. Exiting..")
    exit(1)
else:
    if(args.metallib):
        # Metal Shader Compilation
        air_files = []
        for _f in args.files:
            end_idx = len(_f)-1
            res = os.path.basename(_f)
            res += ".air"
            res = os.path.join(os.path.dirname(args.O),res)
            print(res)
            air_files.append(res)
            os.system('xcrun -sdk macosx metal -c ' + _f + ' -o ' + res)
        spacer = " "
        os.system('xcrun -sdk macosx metallib ' + spacer.join(air_files) + ' -o ' + args.O)


