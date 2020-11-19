# Hardcoded test to verify distance between galaxies can be done
# https://astronomy.stackexchange.com/questions/39595/what-is-the-distance-from-alpha-centauri-to-barnards-star

import math

raStrs = ["14 39", "17 57"]
decStrs = ["-60 50", "+04 41"]
lyLst = [4.37, 5.958]

if len(raStrs) != len(decStrs) or len(raStrs) != len (lyLst):
    error ("Not equal data sizes")

raDegLst = []
for ra in raStrs:
    raDeg = 15 * (int(ra.split(" ")[0]) + float(ra.split(" ")[1])/60)
    raDegLst.append(raDeg)

decDegLst = []
for dec in decStrs:
    decDeg = int(dec.split(" ")[0]) + int(dec.split(" ")[1])/60
    decDegLst.append(decDeg)

print ("ra degrees", raDegLst)
print ("dec degrees", decDegLst)

cordsList = []
for theta, phi, ly in zip(raDegLst, decDegLst, lyLst):
    x = ly * math.cos(math.radians(theta)) * math.cos(math.radians(phi))
    y = ly * math.sin(math.radians(theta)) * math.cos(math.radians(phi))
    z = ly * math.sin(math.radians(phi))
    cord = (x, y, z)
    cordsList.append(cord)

print ("cords light years", cordsList)

print ("Hardcoded 2 variable:")
d = math.sqrt(
    (cordsList[0][0] - cordsList[1][0]) ** 2 +
    (cordsList[0][1] - cordsList[1][1]) ** 2 +
    (cordsList[0][2] - cordsList[1][2]) ** 2
)
print (d)
