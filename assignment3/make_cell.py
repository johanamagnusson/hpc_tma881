import random as rand

NUMBER_OF_POINTS = int(1e5)

def make_string(coordinate):
    string = ""
    if coordinate[0] < 0:
        string += "{0:07.3f} "
    else:
        string += "+{0:06.3f} "
    if coordinate[1] < 0:
        string += "{1:07.3f} "
    else:
        string += "+{1:06.3f} "
    if coordinate[2] < 0:
        string += "{2:07.3f}"
    else:
        string += "+{2:06.3f}"
    return string.format(coordinate[0], coordinate[1], coordinate[2])

def main():
    f = open("cells", "w")
   

    for i in range(NUMBER_OF_POINTS):
        coordinate = [
                -10.0 + 20.0*rand.random(),
                -10.0 + 20.0*rand.random(),
                -10.0 + 20.0*rand.random()
                ]
        f.write(make_string(coordinate) + "\n")
    f.close()

main()
