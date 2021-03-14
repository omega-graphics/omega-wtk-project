import os
import sys
from shutil import which;

#Find PkgConfig
has_pkg_conf:bool

pkg_conf_path = which("pkg-config")
if(pkg_conf_path != None):
    has_pkg_conf = True
else:
    pkg_conf_path = which("pkg-conf")
    if(pkg_conf_path != None):
        has_pkg_conf = True 
    else:
        has_pkg_conf = False
# Parse Argv

if has_pkg_conf == False:
    print("pkg-config nor pkg-conf not found in path")
    sys.stdout.flush();
    exit(0);



def pkg_check_module(pkg:str):
    stream = os.popen(pkg_conf_path + " --print-variables " + pkg)
    data = (stream.read())
    sys.stdout.write(data);

    

args = sys.argv;
args.pop(0);
for arg in args:
    if("--pkg" == arg):
        idx = args.index("--pkg")
        pkg_name = args[idx + 1]
        if(pkg_name == None):
            print("No package name specified exiting...")
            sys.stdout.flush();
            exit(1)
        else:
            pkg_check_module(pkg_name)
