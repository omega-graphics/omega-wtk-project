import os,platform,argparse

parser = argparse.ArgumentParser()
parser.add_argument("--output-dir",type=str)
parser.add_argument("--temp-dir",type=str)
parser.add_argument("--src-dir",type=str)
args = parser.parse_args()

icu_src_dir = args.src_dir
print(icu_src_dir)
first_cmd = ""
if platform.system() == "Windows":
    first_cmd = f"set PYTHONPATH={icu_src_dir}/python;%PYTHONPATH%"

os.chdir(f"{icu_src_dir}/data")
os.system(f"{first_cmd} && py -3 -B -m icutools.databuilder --mode windows-exec --src_dir {icu_src_dir}/data --tool_dir {args.output_dir}/icu-tools --tool_cfg Rel  --out_dir {args.output_dir} --tmp_dir {args.temp_dir}")