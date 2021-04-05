import os,sys,argparse

parser = argparse.ArgumentParser()
parser.add_argument("--assetc",type=str)
parser.add_argument("--dir",type=str)
parser.add_argument("--output",type=str)
args = parser.parse_args()

os.chdir(args.dir)
print(os.getcwd())
all_files = []
for root, dirs, files in os.walk(args.dir):
    for f in files:
        all_files.append(f)

print(all_files)

stream = os.popen(args.assetc + " --output " + args.output + " " + " ".join(all_files))
sys.stdout.write(stream.read())
