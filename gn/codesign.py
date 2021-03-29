import argparse
import os
import shutil


parser = argparse.ArgumentParser()
parser.add_argument("--sig",type=str)
parser.add_argument("--code",type=str)
parser.add_argument("--output_dir",type=str)

args = parser.parse_args()

os.system("codesign --force  -s " + args.sig + " --verbose=3 " + args.code)
# output_file = os.path.join(args.output_dir,os.path.basename(args.code))
# if(output_file):
#     os.remove(output_file)
# shutil.copyfile(args.code,args.output_dir)

