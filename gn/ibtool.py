import argparse
import os

parser = argparse.ArgumentParser()
parser.add_argument("-I",type=str)
parser.add_argument("-O",type=str)
parser.add_argument("-D",type=str)
args = parser.parse_args()

os.system("xcrun -sdk macosx ibtool " + "--compilation-directory " + args.D + "  " + args.I)
