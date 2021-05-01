#!usr/env/python3

import os
import sys
import shutil
import zipfile ,tarfile


def dep(url:str,gitClone:bool,installName:str,branch = "main",fileExt:str = ".zip",custom_commands:"list[str]" = []):
    """
    Defines a dependency
    """
    if custom_commands:
        os.chdir("./external-libs/" + installName)
        os.makedirs("./code")
        for command in custom_commands:
            os.system(command)
    elif gitClone:
        if branch != "main":
            os.system("git clone --branch " + branch + " " + url + " ./external-libs/" + installName + "/code")
        else:   
            os.system("git clone " + url + " ./external-libs/" + installName + "/code")
    else:
        if not os.path.exists("./temp"):
            os.makedirs("./temp")
        os.system("curl " + url + " --output ./temp/" + installName + fileExt)
        if fileExt == ".tar.gz":
            f = tarfile.open(name="./temp/" + installName + fileExt,mode="r:gz")
            f.extractall("./external-libs/" + installName + "/code")
            f.close()
        elif fileExt == ".zip":
            f = zipfile.ZipFile("./temp/" + installName + fileExt,mode="r")
            f.extractall("./external-libs/" + installName + "/code")
            f.close()
def clean_temp_folder():
    shutil.rmtree("./temp")


dep(url="https://github.com/glennrp/libpng.git",gitClone=True,installName="libpng")
dep(url="https://github.com/libjpeg-turbo/libjpeg-turbo.git",gitClone=True,installName="libjpeg-turbo")
dep(url="https://gitlab.com/libtiff/libtiff.git",gitClone=True,installName="libtiff")
dep(url="https://github.com/madler/zlib.git",gitClone=True,installName="zlib")
dep(url="https://ftp.pcre.org/pub/pcre/pcre2-10.36.zip",gitClone=False,installName="pcre2",fileExt=".zip")
dep(url="https://gitlab.gnome.org/GNOME/libxml2.git",gitClone=True,installName="libxml2")
dep(url="https://github.com/unicode-org/icu.git",gitClone=True,installName="icu-69",branch="maint/maint-69")
# dep(url="git://git.openssl.org/openssl.git",gitClone=True,installName="openssl")
# dep(gitClone=False,installName="webrtc",custom_commands=["fetch --nohooks webrtc","gclient sync"])

if os.path.exists("./temp"):
    clean_temp_folder()
