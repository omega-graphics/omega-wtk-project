import os, sys ,shutil
import argparse 
import json
import install_name


def checkOutputs(libs:"list[str]"):
    for l in libs:
        if os.path.exists(l) == False:
            return False
    return True 

def stampFile(file:str):
    str = open(file,"w")
    str.write("CONFIGURED...")
    str.close()

parser = argparse.ArgumentParser()
parser.add_argument("--custom-conf",dest="custom_conf",type=str)
parser.add_argument("--custom-make",dest="custom_make",type=str)
parser.add_argument("--exists",type=str)
parser.add_argument("--make",type=str)
parser.add_argument("--conf",type=str)
parser.add_argument("--build",type=str)
parser.add_argument("--change-install-name",dest="change_install_name",action="store_const",const=True,default=False)
parser.add_argument("--cmake-args",dest="cmake_args",type=str)

parser.add_argument("--check-outputs",dest="check_outputs",nargs="+",type=str)
parser.add_argument("--build-dir",dest="build_dir",type=str)

parser.add_argument("--copy-headers",dest="copy_headers",nargs="+",type=str)
parser.add_argument("--copy-libs",dest="copy_libs",nargs="+",type=str)
parser.add_argument("--copy-headers-dest",dest="copy_headers_dest",type=str)
parser.add_argument("--copy-libs-dest",dest="copy_libs_dest",type=str)

args = parser.parse_known_args()

cmake = "cmake"
if args[0].build_dir :
    os.chdir(args[0].build_dir)
else:
    os.chdir("../gn")

if(args[0].custom_conf):
    if checkOutputs(args[0].check_outputs) == False:
        print("Configuring custom configuration command")
        if(sys.platform == "win32"):
             os.system("vc.bat && " + args[0].custom_conf)
        else:
             os.system(args[0].custom_conf)
        # stampFile(os.path.join(args[0].build,"../CONF"))
elif(args[0].custom_make):
    if checkOutputs(args[0].check_outputs) == False:
        if(sys.platform == "win32"):
            os.system("vc.bat && " + args[0].custom_make)
        else:
            os.system(args[0].custom_make)

    
elif(args[0].exists):
    sys.stdout.write(json.dumps(os.path.exists(args[0].exists)))
elif(args[0].make):
    if checkOutputs(args[0].check_outputs) == False:
        if(sys.platform == "win32"):
            os.system("vc.bat && " +cmake + " --build " + args[0].make)
        else:
            os.system(cmake + " --build " + args[0].make)
        if(args[0].change_install_name):
            install_name.main(args[1])
else: 
    # Configure with CMake
    if checkOutputs(args[0].check_outputs) == False:
        print("Configuring with Cmake. Path:" + cmake)
        
        if(sys.platform == "win32"):
            if(args[0].cmake_args != "\"\""):
                os.system("vc.bat && " + cmake + ' -S ' + args[0].conf + ' -B ' + args[0].build + ' -G"Ninja" -DCMAKE_C_COMPILER=clang-cl -DCMAKE_CXX_COMPILER=clang-cl -DCMAKE_BUILD_TYPE=Release ' + args[0].cmake_args)
            else:
                os.system("vc.bat && " + cmake + ' -S ' + args[0].conf + ' -B ' + args[0].build + ' -G"Ninja" -DCMAKE_C_COMPILER=clang-cl -DCMAKE_CXX_COMPILER=clang-cl -DCMAKE_BUILD_TYPE=Release ')
        else:
            os.system(cmake + ' -S ' + args[0].conf + ' -B ' + args[0].build + ' -G"Ninja" -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ ' + args[0].cmake_args)
        stampFile(os.path.join(args[0].build,"../CONF"))
        
