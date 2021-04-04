import os, sys ,shutil
import argparse 

parser = argparse.ArgumentParser()
parser.add_argument("--make",type=str)
parser.add_argument("--conf",type=str)
parser.add_argument("--build",type=str)

args = parser.parse_args()

cmake = shutil.which("cmake")

if(args.make):
    os.system(cmake + " --build " + args.make)
else: 
    print("Configuring with Cmake. Path:" + cmake)
    stream = os.popen(cmake + ' -S ' + args.conf + ' -B ' + args.build + ' -G"Unix Makefiles"')
    data = (stream.read())
    print(data);
