import os
import sys

def rename(directory):
    for filename in os.listdir(directory + '/tests'):
        if filename.endswith('.a') and not filename.startswith('output_') :
            # rename to output_xxx.txt
            os.rename(directory + '/tests/' + filename, directory + "/tests/output_" + filename[:len(filename) - 1] + "txt")
            print (filename + " is renamed to output_" + filename[:len(filename) - 1] + ".txt")
        elif not filename.startswith('input_') and '.' not in filename:
            # rename to input_xxx.txt
            os.rename(directory + '/tests/' + filename, directory + "/tests/input_" + filename + ".txt")
            print (filename + " should be renamed to input_xxx.txt")

if __name__ == "__main__":
    if len(sys.argv) != 2:
        print ("Please provide the directory you want to test as an argument (e.g. 'PA1/evacuation').")
        sys.exit(0)
    else:
        rename(str(sys.argv[1]))
