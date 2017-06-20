import subprocess

input_filename = "./tests/input_02.txt"
with open(input_filename) as ff:
    content = ff.readlines()
    # print content
    number_of_flights = int(content[0].split(" ")[0])
    number_of_crews = int(content[0].split(" ")[1])
    print number_of_flights, number_of_crews
    M = [[0 for x in range(number_of_crews)] for y in range(number_of_flights)]

    for y in range(number_of_crews - 1):
        # print y
        line = content[y + 1]
        # print line
        for x in range(number_of_flights):
            M[y][x] = int(content[y + 1].split(" ")[x])
    # print M

    command = "./airline_crews.exe " + input_filename
    p = subprocess.Popen(["exec " + command], shell=True, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
    p.wait()
    result = 0

    for line in p.stdout.readlines():
        # print line.decode("utf-8")
        flight_arr = line.decode("utf-8").split(" ")
        for flight in range(0, len(flight_arr)):
            #
            if int(flight_arr[flight]) == -1:
                1
                # print "Skipped"
            elif int(flight_arr[flight]) != -1 and M[int(flight)][int(flight_arr[flight]) - 1] != 1:
                print int(flight), int(flight_arr[flight]) - 1
                print "Error"
            else:
                result = result + 1

    print "sum =", result
    ## Read output file
    output_filename = "./tests/output_02.txt"
    output_result = 0
    with open(output_filename) as of:
        output_list = of.readlines()
        out_put_content = ''.join(output_list)
        flight_arr = out_put_content.split(" ")
        for flight in range (0, len(flight_arr)):
            if flight_arr[int(flight)] != -1:
                output_result = output_result + 1

    print "output sum =", output_result
