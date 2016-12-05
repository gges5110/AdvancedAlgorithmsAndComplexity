import subprocess
import sys
import time
from os import walk

class bcolors:
    HEADER = '\033[95m'
    OKBLUE = '\033[94m'
    OKGREEN = '\033[92m'
    WARNING = '\033[93m'
    FAIL = '\033[91m'
    ENDC = '\033[0m'
    BOLD = '\033[1m'
    UNDERLINE = '\033[4m'

def printWithColor(color, string):
    return color + string + bcolors.ENDC


def main():
    module = ""
    # print 'Number of arguments:', len(sys.argv), 'arguments.'
    # print 'Argument List:', str(sys.argv)
    if len(sys.argv) != 2:
        print "Please provide the module you want to test as an argument."
        sys.exit(0)
    else:
        module = str(sys.argv[1])


    # Get the file list in test directory
    f = []
    for (dirpath, dirnames, filenames) in walk(module + "tests"):
        f.extend(filenames)
        break
    f.sort()

    if len(f) == 0:
        print "No tests found."
        sys.exit(0)

    # TODO: compile code if executable is not there
    print ""
    print "[==========] Running " + str(len(f) / 2) + " test cases."
    passed = 0
    failed = []
    total_time = 0

    # For each pair of tests
    for i in range(0, len(f), 2):
        command = "./" + module + "a.out " + module + "tests/" + f[i]

        test_name = "tests/" + f[i]
        # print command
        print printWithColor(bcolors.OKGREEN, "[ RUN      ] ") + module + test_name

        ts_start = time.time()
        p = subprocess.Popen([command], shell=True, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
        ts_total = time.time() - ts_start
        total_time = total_time + ts_total

        for line in p.stdout.readlines():
            content = ""
            with open(module + "tests/" + f[i + 1]) as ff:
                content = ff.readlines()


            if line == content[0]:
                print printWithColor(bcolors.OKGREEN, "[       OK ] ") + module + test_name + " (" + str(int(ts_total * 1000)) + " ms)"
                passed = passed + 1
            else:
                print printWithColor(bcolors.FAIL, "Failure ")
                print "Expected: " + printWithColor(bcolors.OKGREEN, content[0].rstrip())
                print "Actual:   " + printWithColor(bcolors.FAIL, line.rstrip())
                failed.append(test_name)
    print ""
    print printWithColor(bcolors.OKGREEN, "[==========] ") + str(len(f) / 2) + " tests ran. (" + str(int(total_time * 1000)) + " ms)"
    print printWithColor(bcolors.OKGREEN, "[  PASSED  ] ") + str(passed) + " tests."
    if len(failed) > 0:
        print printWithColor(bcolors.FAIL, "[  FAILED  ] ") + str(len(failed)) + " tests, listed below:"

        for name in failed:
            print printWithColor(bcolors.FAIL, "[  FAILED  ] ") + name
    print ""

if __name__ == "__main__":
    main()
