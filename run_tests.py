#!/usr/bin/env python3
from subprocess import check_output, DEVNULL
import sys


is_ci = sys.argv[1] == 'ci'

def getAllTestFiles() -> list:
    from os import listdir
    from os.path import isfile, join    
    test_path=".\Team36\Tests36"
    return [f for f in listdir(test_path) if isfile(join(test_path, f))]
def getAllTestCases(tc_files):
    return sorted(list(set([int(f.split('_')[0]) for f in tc_files])))


tc_files = getAllTestFiles()
tc_description_map = {int(f.split('_')[0]): f.split('_')[1] for f in tc_files if 'source' in f}

def parseArg(arg: str):
    if arg.isdigit():
        return [int(arg)]
    if arg == 'all' or arg == 'ci':
        return getAllTestCases(tc_files)
    if '-' in arg:
        l = arg.split('-')
        return range(int(l[0]), int(l[1]) + 1)
    if ',' in arg:
        l = arg.split(',')
        return [int(i) for i in l]
    print("Invalid Args")
    exit(1)
    

# CHECK
if len(sys.argv) < 2:
    print("\nExample Usage:")
    print("\tpython .\\run_tests.py 1-5            \t# Run Testcase 1-5")
    print("\tpython .\\run_tests.py 1,2            \t# Run Testcase 1 and 2")
    print("\tpython .\\run_tests.py 1              \t# Run Testcase 1")
    print("\tpython .\\run_tests.py 1-15 105-106   \t# Run Testcase 1-5 and 105-106\n")
    exit(1)

unflattened_testcases = [parseArg(sys.argv[i]) for i in range(1,len(sys.argv))]
testcases = [i for sublst in unflattened_testcases for i in sublst]

# BUILD
print("Building...", end="",flush=True)
if is_ci == False:
    from os import path
    possible_locations = [
        "C:\Program Files (x86)\Microsoft Visual Studio\\2019\Community\MSBuild\Current\Bin\MSBuild.exe",
        "C:\Program Files\Microsoft Visual Studio\\2022\Community\Msbuild\Current\Bin\MSBuild.exe",
    ]
    msbuild=''
    for p in possible_locations:
        if path.exists(p): 
            msbuild = p
            break
    if not msbuild:
        print("MSBuild not found.")
        exit(1)
    sln_path =".\Team36\Code36\StartupSPASolution.sln"
    if not "Build succeeded." in str(check_output((msbuild, sln_path))): 
        print("Build Failed.")
        exit(1) 
print("Done.")


# TEST
from time import time
autotester = ".\Team36\Code36\Debug\AutoTester.exe"
score = 0
start_time = time()
for i in testcases:
    test_cmd = f"{autotester} .\Team36\Tests36\{i}_{tc_description_map[i]}_source.txt .\Team36\Tests36\{i}_{tc_description_map[i]}_queries.txt .\Team36\Code36\Tests\out.xml"
    output = check_output(test_cmd, stderr=DEVNULL).decode()
    if "Missing:" in output:
        if is_ci:
            raise Exception(f"[TEST CASE {i}] [{tc_description_map[i]}] FAILED")
        else:
            print(f"[TEST CASE {i}] [{tc_description_map[i]}] FAILED -----> Log File : ./TEST_CASE_{i}.out")
            with open(f"TEST_CASE_{i}.out", "w") as f: f.write(output)
    else:
        score+=1
        print(f"[TEST CASE {i}] PASSED")

end_time = time()
print(f"Score : {score} / {len(testcases)}")
print(f"Time Taken : {end_time - start_time}s")


