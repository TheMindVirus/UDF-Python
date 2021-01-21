#-------------------------------------------------------#
# gen_inf_headers.py - Partial UMDF .inf File Generator #
# Alastair Cota - 21/02/2021                            #
#-------------------------------------------------------#
import os, hashlib

DIRECTORY = os.getcwd() + "\\CopyFiles"
OUTPUTFILE = ".\\gen_inf_headers.txt"
FILEFLAGS = ",,,0x00004000"

UMDRIVERCOPY = "UMDriverCopy"
MODROOTPREFIX = "ModuleRoot"
MODPREFIX = "Module_"

PROJECTNAME = "UDF"
INSTALLROOT = "UMDF"
DSTROOT = "Python"
DSTDIRID = "12"

#-------------------------------------------------------#

def AliSHA1(data):
    method = hashlib.sha1()
    method.update(bytes(data, "utf-8"))
    return method.hexdigest()

#-------------------------------------------------------#

modules = []
modules.append({"srcdisk": 1,
                "name": UMDRIVERCOPY,
                "path": "",
                "files": [PROJECTNAME + ".dll"]})
directory = os.walk(DIRECTORY)
output = open(OUTPUTFILE, "w")

i = 2
output.write("\n[DestinationDirs]\n")
output.write("DefaultDestDir=" + DSTDIRID + "\n")
output.write(modules[0]["name"] + "="
             + DSTDIRID + ",\""
             + INSTALLROOT + "\"\n")
for path, subdirs, files in directory:
    name = ""
    relative = ""
    if i == 2:
        name = MODROOTPREFIX
        relative = DSTROOT
        output.write(name + "="
                     + DSTDIRID + ",\""
                     + INSTALLROOT + "\\"
                     + relative + "\"\n")
    else:
        prefix = os.path.commonprefix([DIRECTORY, path])
        relative = DSTROOT + "\\" + os.path.relpath(path, prefix)
        name = MODPREFIX + AliSHA1(relative)
        value = DSTDIRID + ",\"" + INSTALLROOT + "\\" + relative + "\""
        output.write(name + "=" + value + "\n")
    modules.append({"srcdisk": i,
                    "name": name,
                    "path": relative,
                    "files": files})
    i = i + 1

output.write("\n[SourceDisksNames]\n")
for module in modules:
    output.write(str(module["srcdisk"]) + "=\""
                 + module["name"] + "\",,,\"\\"
                 + module["path"] + "\"\n")

output.write("\n[SourceDisksFiles]\n")
for module in modules:
    for filename in module["files"]:
        output.write("\"" + filename + "\"" + "="
                     + str(module["srcdisk"]) + "\n")

output.write("\n[" + PROJECTNAME + "_Device.NT]\n")
for module in modules:
    output.write("CopyFiles=" + module["name"] + "\n")
output.write("\n")

for module in modules:
    output.write("[" + module["name"] + "]\n")
    for filename in module["files"]:
        output.write("\"" + filename + "\"" + FILEFLAGS + "\n")
    output.write("\n")

print("Done!")
output.close()

#-------------------------------------------------------#
