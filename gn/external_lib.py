import os, sys ,shutil
import argparse 
import json
import install_name

parser = argparse.ArgumentParser()
parser.add_argument("--exists",type=str)
parser.add_argument("--make",type=str)
parser.add_argument("--conf",type=str)
parser.add_argument("--build",type=str)
parser.add_argument("--change-install-name",dest="change_install_name",action="store_const",const=True,default=False)

args = parser.parse_known_args()

cmake = "cmake"

os.chdir("../gn")

if(args[0].exists):
    sys.stdout.write(json.dumps(os.path.exists(args[0].exists)))
elif(args[0].make):
    if(sys.platform == "win32"):
        os.system("vc.bat && " +cmake + " --build " + args[0].make)
    else:
        os.system(cmake + " --build " + args[0].make)
    if(args[0].change_install_name):
        install_name.main(args[1])
else: 
    print("Configuring with Cmake. Path:" + cmake)
    
    if(sys.platform == "win32"):
        stream = os.popen("vc.bat && " + cmake + ' -S ' + args[0].conf + ' -B ' + args[0].build + ' -G"Ninja" -DCMAKE_C_COMPILER=clang-cl -DCMAKE_CXX_COMPILER=clang-cl -DCMAKE_BUILD_TYPE=Release')
    else:
        stream = os.popen(cmake + ' -S ' + args[0].conf + ' -B ' + args[0].build + ' -G"Ninja" -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++')

    data = (stream.read())
    print(data);
