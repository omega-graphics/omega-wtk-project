import os 
import argparse
import sys



parser = argparse.ArgumentParser()
parser.add_argument("-F",type=str)
parser.add_argument("--name",type=str)
parser.add_argument("--current_version",type=str)
parser.add_argument("--check_links",dest="check_links",action="store_const",const=True,default=False)
parser.add_argument("--symlink-other-dirs",dest="symlink_other_dirs",nargs="+",type=str)

def main(_args:"list[str]"):
    args = parser.parse_args(_args)

    framework_main_dir = args.F
    framework_current_version_dir = args.F + "/" + "Versions" + "/" + args.current_version

    if(args.check_links):
        sys.stdout.write("{}".format(os.path.exists(framework_main_dir + "/" + args.name) and os.path.exists(framework_main_dir + "/" + "Resources") and os.path.exists(framework_main_dir + "/" + "Versions" + "/" + "Current")))
    else:
        # Generate Symbolic Link For Current Version
        if(os.path.exists(framework_main_dir + "/" + "Versions" + "/" + "Current") == False):
            os.symlink(framework_current_version_dir,framework_main_dir + "/" + "Versions" + "/" + "Current")
        # Generate Symbolic Link For Framework Lib..
        current_version = framework_main_dir + "/" + "Versions" + "/" + "Current"
        if(os.path.exists(framework_main_dir + "/" + args.name) == False):
            os.symlink(current_version + "/" + args.name,framework_main_dir + "/" + args.name)
        # Generate Symbolic Link for Resources
        if(os.path.exists(framework_main_dir + "/" + "Resources") == False):
            os.symlink(current_version + "/" + "Resources",framework_main_dir + "/" + "Resources")

        if(args.symlink_other_dirs):
            for d in args.symlink_other_dirs:
                if(os.path.exists(framework_main_dir + "/" + d) == False):
                    os.symlink(current_version + "/" + d,framework_main_dir + "/" + d)
        
if(__name__ == "__main__"):
    main(sys.argv)


