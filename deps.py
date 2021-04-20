#!usr/env/python3

import os
import sys
import shutil

def dep(url:str,gitClone:bool,installName:str,fileExt:str = ".zip",custom_commands:"list[str]" = []):
    """
    Defines a dependency
    """
    if custom_commands:
        os.chdir("./external-libs/" + installName)
        os.makedirs("./code")
        for command in custom_commands:
            os.system(command)
    elif gitClone:
        os.system("git clone " + url + " ./external-libs/" + installName + "/code")
    else:
        if not os.path.exists("./temp"):
            os.makedirs("./temp")
        os.system("curl " + url + " --output ./temp/" + installName + fileExt)
        if fileExt == ".zip":
            os.makedirs("./external-libs/" + installName + "/code")
            os.system("tar -xf " + "./temp/" + installName + fileExt + " -C " + "./external-libs/" + installName + "/code")

def clean_temp_folder():
    shutil.rmtree("./temp")


dep(url="https://github.com/glennrp/libpng.git",gitClone=True,installName="libpng")
dep(url="https://github.com/libjpeg-turbo/libjpeg-turbo.git",gitClone=True,installName="libjpeg-turbo")
dep(url="https://gitlab.com/libtiff/libtiff.git",gitClone=True,installName="libtiff")
dep(url="https://github.com/madler/zlib.git",gitClone=True,installName="zlib")
dep(url="https://ftp.pcre.org/pub/pcre/pcre2-10.36.zip",gitClone=False,installName="pcre2",fileExt=".zip")
# dep(url="git://git.openssl.org/openssl.git",gitClone=True,installName="openssl")
dep(gitClone=False,installName="webrtc",custom_commands=["gclient config","fetch --nohooks webrtc","gclient sync"])

if os.path.exists("./temp"):
    clean_temp_folder()
