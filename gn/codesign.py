import argparse
import os
import shutil
import sys
import AppleFramework


parser = argparse.ArgumentParser()
parser.add_argument("--sig",type=str)
parser.add_argument("--code",type=str)
parser.add_argument("--output_dir",type=str)
parser.add_argument("--framework",dest="framework",action="store_const",const=True,default=False)
args = parser.parse_known_args()

if(args[0].framework):
    AppleFramework.main(args[1])

os.system("codesign --force  -s " + args[0].sig + " --verbose=3 " + args[0].code)
# output_file = os.path.join(args.output_dir,os.path.basename(args.code))
# if(output_file):
#     os.remove(output_file)
# shutil.copyfile(args.code,args.output_dir)

