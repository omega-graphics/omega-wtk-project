import argparse
import os

def main(args):

    parser = argparse.ArgumentParser()
    parser.add_argument("--lib",type=str)
    parser.add_argument("--old-name",dest="old_name",type=str)
    parser.add_argument("--new-name",dest="new_name",type=str)
    args = parser.parse_args(args)

    stream = os.popen("install_name_tool -change " + args.old_name + " " + args.new_name + " " + args.lib)
    data = stream.read()
    print(data)
