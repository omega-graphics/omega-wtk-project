import os,argparse,sys


parser = argparse.ArgumentParser()
parser.add_argument("--src-dir",type=str,nargs="?")
parser.add_argument("--tmp-dir",type=str,nargs="?")
parser.add_argument("--out-dir",type=str,nargs="?")
parser.add_argument("--config",type=str,nargs="?")
parser.add_argument("--build",type=str,nargs="?")
parser.add_argument("--install",type=str,nargs="?")

args = parser.parse_args()

if args.src_dir:
    src_dir = os.path.abspath(args.src_dir)

if args.tmp_dir:
    tmp_dir = os.path.abspath(args.tmp_dir)

if args.out_dir:
    dest_dir = os.path.abspath(args.out_dir)

if args.src_dir:
    os.chdir(src_dir)
if args.config:
    os.system(args.config)
if args.tmp_dir:
    os.chdir(tmp_dir)

if args.build:
    os.system(args.build)
if args.install:
    os.system(args.install)