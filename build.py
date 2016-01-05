#! /usr/bin/env python

import argparse
import platform
import glob
import os
import errno
import shutil
import filecmp
from subprocess import call

def mkdir_p(path):
    try:
        os.makedirs(path)
    except OSError as exc:
        if exc.errno == errno.EEXIST and os.path.isdir(path):
            pass
        else: raise


parser = argparse.ArgumentParser(description="Compile Strong.")
parser.add_argument("-a", "--all", action="store_true")
parser.add_argument("-l", "--linkonly", action="store_true")
parser.add_argument("-r", "--run", action="store_true")
parser.add_argument("-o", "--optimize", action="store_true")
parser.add_argument("-d", "--debug", action="store_true")
parser.add_argument("-w", "--windows", action="store_true")
parser.add_argument("-s", "--server", action="store_true")
parser.add_argument("-c", "--client", action="store_true")
parser.add_argument("-n", "--defaultNetwork", action="store_true")
parser.add_argument("--compiler", help="specify compiler")

args = parser.parse_args()

systemName = platform.system()

sourceDirectory = "src"
objectDirectory = "obj"
copyDirectory = "cpy"

sourceExtention = "cpp"
headerExtention = "hpp"
objectExtention = "opp"


compiler = "g++"
compilerFlags = "-std=gnu++11"
libraryFlags = ""

optimization = "-g -O0 -Wall"

executableName = "Strong"

compileAll = args.all


if args.optimize:
    optimization = "-O3"
    executableName += "_Optim"

compilerFlags = optimization+" "+compilerFlags;



if args.compiler:
    compiler = args.compiler;


if args.windows:

    if systemName == "Darwin":
        compiler = "i586-mingw32-c++"
        compilerFlags += " -D WIN32"
    elif systemName == "Linux":
        compiler = "i686-w64-mingw32-c++"
        compilerFlags += " -D WIN32"

    libraryFlags = "-lSDL2main -lSDL2 -lSDL2_image -lSDL2_net"
    print("    # The Windows executable will require the following dll files:")
    print("    #   SDL2.dll")
    print("    #   SDL2_image.dll")
    print("    #   SDL2_net.dll")
    print("    #   zlib1.dll")
    print("    #   libpng16-16.dll")
else:
    if systemName == "Darwin":
        compilerFlags += " -I/usr/local/include"
        libraryFlags = "-L/usr/local/lib `sdl2-config --cflags` -lSDL2_image -lSDL2_net"
        libraryFlags += " `sdl2-config --libs`"
    elif systemName == "Linux":
        compilerFlags += " `sdl2-config --cflags`"
        libraryFlags  = " `sdl2-config --libs` -lSDL2_image -lSDL2_net"

if args.windows:
    executableName += "_Windows.exe"
elif systemName == "Darwin":
    executableName += "_OSX"
else:
    executableName += "_"+systemName




objectDirectory += "/"+executableName
copyDirectory = objectDirectory+copyDirectory

cppList = [a.replace("\\", "/") for a in sorted(glob.glob(sourceDirectory+"/*."+sourceExtention))]
hppList = [a.replace("."+sourceExtention, "."+headerExtention) for a in cppList]
oppList = [a.replace(sourceDirectory+"/", objectDirectory+"/") for a in [a.replace("."+sourceExtention, "."+objectExtention) for a in cppList]]
cppCopyList = [a.replace(sourceDirectory+"/", copyDirectory+"/") for a in cppList]
hppCopyList = [a.replace(sourceDirectory+"/", copyDirectory+"/") for a in hppList]


mkdir_p(objectDirectory)
mkdir_p(copyDirectory)

returnCode = 0

if os.path.isfile(executableName):
    print("    * Renaming old executable to: "+executableName+"_outdated")
    if os.path.isfile(executableName+"_outdated"):
        os.remove(executableName+"_outdated")
    os.rename(executableName, executableName+"_outdated")

skipCount = 0
    
print("    # Using compiler: "+compiler)

if not args.linkonly:
    print("    # Building: "+executableName);

    for i in range(len(cppList)):

        cppDiff = True
        hppDiff = True

        hppExists = os.path.isfile(hppList[i])

        if os.path.isfile(cppCopyList[i]):
            cppDiff = not filecmp.cmp(cppList[i], cppCopyList[i])

        if hppExists:
            if os.path.isfile(hppCopyList[i]):
                hppDiff = not filecmp.cmp(hppList[i], hppCopyList[i])
        else:
            hppDiff = False


        if compileAll or cppDiff or hppDiff:
            print("    + Compiling: "+cppList[i])
            command = compiler+" "+compilerFlags+" -c "+cppList[i]+" -o "+oppList[i]
            if args.debug:
                print("    . Executing: \""+command+"\"")
            returnCode = call(command, shell=True)
            if returnCode == 0:
                shutil.copy(cppList[i], cppCopyList[i])
                if hppExists:
                    shutil.copy(hppList[i], hppCopyList[i])
            else:
                break
        else:
            skipCount += 1


print("    . Skipped "+str(skipCount)+" compilations.")

if returnCode == 0:
    print("    ~ Linking: "+executableName);
    command = compiler+" "+compilerFlags+" "+(" ".join(oppList))+" -o "+ executableName+" "+libraryFlags
    if args.debug:
        print("    . Executing: \""+command+"\"")
    returnCode = call(command, shell=True)

    if returnCode == 0:
        if os.path.isfile(executableName+"_outdated") and os.path.isfile(executableName):
            print("    - Deleteing outdated executable: "+executableName+"_outdated")
            os.remove(executableName+"_outdated")
        if args.run and returnCode == 0:
            print("    > Running...")
            
            runCommand = executableName
            
            if args.server:
                runCommand += " s"
                if args.defaultNetwork:
                    runCommand += " 1337"
            elif args.client:
                runCommand += " c"
                if args.defaultNetwork:
                    runCommand += " localhost:1337 nedearb"
            
            if systemName == "Windows":
                call(runCommand, shell=True)
            else:
                call("./"+runCommand, shell=True)
            
    else:
        print("    x Failed to link, Error Code: "+str(returnCode))

else:
    print("    x Failed to compile, Error Code: "+str(returnCode))
