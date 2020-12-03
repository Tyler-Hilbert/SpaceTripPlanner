# Creates edges between galaxies where a space ship could travel between
# Outputs:
#  output.gv -- The graph in a format that can be turned into a png using:
#    `$dot -Tpng output.gv -o output.png`
#    // After installing graphviz `$brew install graphviz`
#  graph_variables.cu -- The graph data output into a C/CUDA file already instantiated
#    This C/CUDA code will need to be copied into the kernel source file

import csv
import math

MAXIMUM_DISTANCE = 1 # The maximum number of light years a spaceship can travel before needing a refuel
GRAPH_NUMERIC = False # If the output GV file should output id number instead of galaxy names

# Get the X, Y, Z cordinates of a galaxy
def getCord(gal):
    theta = 15 * (int(gal['RA'].split(" ")[0]) + float(gal['RA'].split(" ")[1])/60)
    phi = int(gal['DEC'].split(" ")[0]) + int(gal['DEC'].split(" ")[1])/60
    x = float(gal['LY']) * math.cos(math.radians(theta)) * math.cos(math.radians(phi))
    y = float(gal['LY']) * math.sin(math.radians(theta)) * math.cos(math.radians(phi))
    z = float(gal['LY']) * math.sin(math.radians(phi))
    cord = (x, y, z)
    return cord


# Calculates the distance between 2 galaxies
def calcDistance(startGal, endGal):
    startCord = getCord(startGal)
    endCord = getCord(endGal)
    return math.sqrt(
        (startCord[0] - endCord[0]) ** 2 +
        (startCord[1] - startCord[1]) ** 2 +
        (startCord[2] - startCord[2]) ** 2
    )

# Counts the number of edges with the given start node
# This is used for outputting the length in cuda
def getStartCount(edges, start):
    count = 0
    for edge in edges:
        if edge['start'] == start:
            count += 1
    return count

# Cleans up galaxy name so it will work with DOT
def cleanName(name):
    replacements = [
        (".", ""),
        ("/", "_"),
        ("'", ""),
        ("=", "_"),
        ("-", ""),
        (" ", "_"),
        (" ", "_"),
        ("_", "")
    ]
    for repl in replacements:
        while repl[0] in name:
            name = name.replace(repl[0], repl[1])
    return name


# Read in galaxy data
galaxies = []
with open('GalaxiesWithDistances.csv', 'r') as file:
    reader = csv.reader(file)
    first = True
    for row in reader:
        if first:
            first = False
            continue
        galaxies.append({'Name': row[0], 'RA': row[1], "DEC": row[2], "LY": row[3]})

# Calculate edges
edges = []
for startGalIndex in range(len(galaxies)):
    for endGalIndex in range(len(galaxies)):
        if startGalIndex != endGalIndex and calcDistance(galaxies[startGalIndex], galaxies[endGalIndex]) < MAXIMUM_DISTANCE:
            edge = {'start':"{0:0=2d}".format(startGalIndex), 'end':"{0:0=2d}".format(endGalIndex)}
            edges.append(edge)

# Print
print ("********** \n EDGES \n******* \n ")
for edge in edges:
    print (edge)

# Print graph in CUDA code
f = open("graph_variables.cu", "w")
f.write ("// THIS IS A GENERATED FILE USING EdgeCreator.py\n")
f.write ("// Number of lightyears a spaceship can travel: ")
f.write (str(MAXIMUM_DISTANCE))
f.write ("\n")
f.write("Node node[")
f.write(str(len(galaxies)))
f.write("];\n")

completedStart = []
for i in range(len(edges)):
    if edges[i]['start'] not in completedStart:
        completedStart.append(edges[i]['start'])
        startS = "node[" + str(edges[i]['start']) + "].start = " + str(i) + ";\n"
        f.write (startS)
        lengthS = "node[" + str(edges[i]['start']) + "].length = " + str(getStartCount(edges, edges[i]['start'])) + ";\n"
        f.write (lengthS)


for i in range(len(galaxies)):
    nameS = 'node[' + str(i) + '].name = "' + galaxies[i]['Name'].replace("'", "") + '";\n'
    f.write (nameS)
    id0 = str( (int)((i - (i%10))/10))
    id1 = str(i % 10)
    idS = "node[" + str(i) + "].id[0] = '" + id0 + "'; node[" + str(i) + "].id[1] = '" + id1 + "';\n"
    f.write (idS)


f.write("int edges[")
f.write(str(len(edges)))
f.write("];\n")

for i in range(len(edges)):
    edgeS = "edges[" + str(i) + "] = " + str(edges[i]['end']) + ";\n"
    f.write (edgeS)

f.close()


# Print graph in regular C++ datastructure for sequential BFS (for comparison to CUDA)
f = open("graph_variables.cpp", "w")
f.write ("// THIS IS A GENERATED FILE USING EdgeCreator.py\n")
f.write ("// Number of lightyears a spaceship can travel: ")
f.write (str(MAXIMUM_DISTANCE))
f.write ("\n")

# Add nodes
for i in range(len(galaxies)):
    nameS = 'graph.AddNode("' + galaxies[i]['Name'].replace("'", "") + '");\n'
    f.write (nameS)

# Add edges
for edge in edges:
    edgeS = "graph.AddEdge(" +  edge['start'] + "," + edge['end'] + ");\n"
    f.write(edgeS)

f.close()

# Graph (dot) creator
f = open("output.gv", "w")
f.write("strict graph G {\n")
for edge in edges:
    if GRAPH_NUMERIC:
        eStr = edge['start'] + " -- " + edge['end'] + "\n"
    else:
        eStr = cleanName(galaxies[int(edge['start'])]['Name'])
        eStr += " -- "
        eStr += cleanName(galaxies[int(edge['end'])]['Name'])
        eStr += "\n"
    f.write(eStr)
f.write("}")
f.close()
