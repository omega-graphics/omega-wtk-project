import distutils.spawn as DS
import os

third_party_libs = {
    "libpng":"https://github.com/glennrp/libpng.git",
    # "ffmpeg":"https://github.com/FFmpeg/FFmpeg.git",
    # "libjpeg-turbo":"https://github.com/libjpeg-turbo/libjpeg-turbo.git"
    }

gitpath = DS.find_executable("git")
if(gitpath != None):
    os.chdir(os.path.join(os.getcwd(),"external-deps"))
    for lib in third_party_libs:
        exec_command = "git clone " + third_party_libs[lib] + " " + lib + "/code"
        print("Cloning " + lib)
        os.system(exec_command)
else:
    raise Exception("Git not Found in Path. Exiting..")