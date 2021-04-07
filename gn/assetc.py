import os,sys,argparse

parser = argparse.ArgumentParser()
parser.add_argument("--assetc",type=str)
parser.add_argument("--dir",type=str)
parser.add_argument("--output",type=str)
parser.add_argument("--app-id",dest="app_id",type=str)
args = parser.parse_args()

os.chdir(args.dir)
print(os.getcwd())
all_files = []
for root, dirs, files in os.walk(args.dir):
    for f in files:
        all_files.append(f)

print(all_files)

os.system(args.assetc + " --output " + args.output + " --application-id " + args.app_id + " " + " ".join(all_files))

