import os,argparse,sys


parser = argparse.ArgumentParser()
parser.add_argument("--src-dir",type=str)
parser.add_argument("--tmp-dir",type=str)
parser.add_argument("--out-dir",type=str)
parser.add_argument("--config",type=str)
parser.add_argument("--build",type=str)
parser.add_argument("--install",type=str)

args = parser.parse_args()

src_dir = os.path.abspath(args.src_dir)
tmp_dir = os.path.abspath(args.tmp_dir)
dest_dir = os.path.abspath(args.out_dir)

os.chdir(src_dir)
os.system(args.config)
os.chdir(tmp_dir)
os.system(args.build)
os.system(args.install)