import subprocess
import sys
from os import walk

def test(input_filename, output_filename):
    input_filename = "./tests/input_02.txt"
    with open(input_filename) as ff:
        content = ff.readlines()
        # print content
        number_of_flights = int(content[0].split(" ")[0])
        number_of_crews = int(content[0].split(" ")[1])
        M = [[0 for x in range(number_of_crews)] for y in range(number_of_flights)]

        for y in range(0, number_of_flights - 1):
            line = content[y + 1]
            for x in range(0, number_of_crews - 1):
                M[y][x] = int(content[y + 1].split(" ")[x])
        # print M

        command = "./airline_crews.exe " + input_filename
        p = subprocess.Popen(["exec " + command], shell=True, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
        p.wait()
        input_result = 0

        for line in p.stdout.readlines():
            flight_arr = line.decode("utf-8").split(" ")
            for flight in range(0, len(flight_arr)):
                #
                if int(flight_arr[flight]) == -1:
                    1
                elif int(flight_arr[flight]) == -1 and M[int(flight)][int(flight_arr[flight]) - 1] != -1:
                    # print ("Error")
                    1
                else:
                    input_result = input_result + 1

        ## Read output file
        output_filename = "./tests/output_02.txt"
        output_result = 0
        with open(output_filename) as of:
            output_list = of.readlines()
            out_put_content = ''.join(output_list)
            flight_arr = out_put_content.split(" ")
            for flight in range (0, len(flight_arr)):
                if flight_arr[int(flight)] != '-1':
                    output_result = output_result + 1

        return output_result == input_result

if __name__ == '__main__':
    # Get the file list in test directory
    f = []
    # test_folder = raw_input("test folder?")
    for (dirpath, dirnames, filenames) in walk("./tests"):
    # for (dirpath, dirnames, filenames) in walk(test_folder):
        for input_filename in filenames:
            if input_filename[0:5] == 'input':
                f.append(input_filename)
        break
    f.sort()

    passed_tests = 0
    for i in range(0, len(f)):
        test_name = "tests/" + f[i]
        output_filename = "/tests/output" + str(f[5:])
        if test(test_name, output_filename):
            passed_tests = passed_tests + 1

    print ("Total tests = " + str(len(f)))
    print ("Passed tests = " + str(passed_tests))
