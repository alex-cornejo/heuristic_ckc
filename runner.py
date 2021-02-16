import os

# build
os.system("cmake -DCMAKE_BUILD_TYPE=Release .")
os.system("cmake --build .")

# small instances dataset 2
# 100 experiments
# experiments = [
#
#     # kroA200
#     # PH
#     "bin/ckc_heuristic '/mnt/c/Users/Alex/Documents/inaoe/repos/heuristic_ckc/dataset/kroA200.tsp' 200 5 39 100 1 false tsplib",
#     "bin/ckc_heuristic '/mnt/c/Users/Alex/Documents/inaoe/repos/heuristic_ckc/dataset/kroA200.tsp' 200 5 40 100 1 false tsplib",
#     "bin/ckc_heuristic '/mnt/c/Users/Alex/Documents/inaoe/repos/heuristic_ckc/dataset/kroA200.tsp' 200 10 19 100 1 false tsplib",
#     "bin/ckc_heuristic '/mnt/c/Users/Alex/Documents/inaoe/repos/heuristic_ckc/dataset/kroA200.tsp' 200 10 20 100 1 false tsplib",
#     "bin/ckc_heuristic '/mnt/c/Users/Alex/Documents/inaoe/repos/heuristic_ckc/dataset/kroA200.tsp' 200 20 9 100 1 false tsplib",
#     "bin/ckc_heuristic '/mnt/c/Users/Alex/Documents/inaoe/repos/heuristic_ckc/dataset/kroA200.tsp' 200 20 10 100 1 false tsplib",
#     "bin/ckc_heuristic '/mnt/c/Users/Alex/Documents/inaoe/repos/heuristic_ckc/dataset/kroA200.tsp' 200 40 4 100 1 false tsplib",
#     "bin/ckc_heuristic '/mnt/c/Users/Alex/Documents/inaoe/repos/heuristic_ckc/dataset/kroA200.tsp' 200 40 5 100 1 false tsplib",
#     # PH+
#     "bin/ckc_heuristic '/mnt/c/Users/Alex/Documents/inaoe/repos/heuristic_ckc/dataset/kroA200.tsp' 200 5 39 1 200 false tsplib",
#     "bin/ckc_heuristic '/mnt/c/Users/Alex/Documents/inaoe/repos/heuristic_ckc/dataset/kroA200.tsp' 200 5 40 1 200 false tsplib",
#     "bin/ckc_heuristic '/mnt/c/Users/Alex/Documents/inaoe/repos/heuristic_ckc/dataset/kroA200.tsp' 200 10 19 1 200 false tsplib",
#     "bin/ckc_heuristic '/mnt/c/Users/Alex/Documents/inaoe/repos/heuristic_ckc/dataset/kroA200.tsp' 200 10 20 1 200 false tsplib",
#     "bin/ckc_heuristic '/mnt/c/Users/Alex/Documents/inaoe/repos/heuristic_ckc/dataset/kroA200.tsp' 200 20 9 1 200 false tsplib",
#     "bin/ckc_heuristic '/mnt/c/Users/Alex/Documents/inaoe/repos/heuristic_ckc/dataset/kroA200.tsp' 200 20 10 1 200 false tsplib",
#     "bin/ckc_heuristic '/mnt/c/Users/Alex/Documents/inaoe/repos/heuristic_ckc/dataset/kroA200.tsp' 200 40 4 1 200 false tsplib",
#     "bin/ckc_heuristic '/mnt/c/Users/Alex/Documents/inaoe/repos/heuristic_ckc/dataset/kroA200.tsp' 200 40 5 1 200 false tsplib",
#
#     # kroB200
#     # PH
#     "bin/ckc_heuristic '/mnt/c/Users/Alex/Documents/inaoe/repos/heuristic_ckc/dataset/kroB200.tsp' 200 5 39 100 1 false tsplib",
#     "bin/ckc_heuristic '/mnt/c/Users/Alex/Documents/inaoe/repos/heuristic_ckc/dataset/kroB200.tsp' 200 5 40 100 1 false tsplib",
#     "bin/ckc_heuristic '/mnt/c/Users/Alex/Documents/inaoe/repos/heuristic_ckc/dataset/kroB200.tsp' 200 10 19 100 1 false tsplib",
#     "bin/ckc_heuristic '/mnt/c/Users/Alex/Documents/inaoe/repos/heuristic_ckc/dataset/kroB200.tsp' 200 10 20 100 1 false tsplib",
#     "bin/ckc_heuristic '/mnt/c/Users/Alex/Documents/inaoe/repos/heuristic_ckc/dataset/kroB200.tsp' 200 20 9 100 1 false tsplib",
#     "bin/ckc_heuristic '/mnt/c/Users/Alex/Documents/inaoe/repos/heuristic_ckc/dataset/kroB200.tsp' 200 20 10 100 1 false tsplib",
#     "bin/ckc_heuristic '/mnt/c/Users/Alex/Documents/inaoe/repos/heuristic_ckc/dataset/kroB200.tsp' 200 40 4 100 1 false tsplib",
#     "bin/ckc_heuristic '/mnt/c/Users/Alex/Documents/inaoe/repos/heuristic_ckc/dataset/kroB200.tsp' 200 40 5 100 1 false tsplib",
#     # PH+
#     "bin/ckc_heuristic '/mnt/c/Users/Alex/Documents/inaoe/repos/heuristic_ckc/dataset/kroB200.tsp' 200 5 39 1 200 false tsplib",
#     "bin/ckc_heuristic '/mnt/c/Users/Alex/Documents/inaoe/repos/heuristic_ckc/dataset/kroB200.tsp' 200 5 40 1 200 false tsplib",
#     "bin/ckc_heuristic '/mnt/c/Users/Alex/Documents/inaoe/repos/heuristic_ckc/dataset/kroB200.tsp' 200 10 19 1 200 false tsplib",
#     "bin/ckc_heuristic '/mnt/c/Users/Alex/Documents/inaoe/repos/heuristic_ckc/dataset/kroB200.tsp' 200 10 20 1 200 false tsplib",
#     "bin/ckc_heuristic '/mnt/c/Users/Alex/Documents/inaoe/repos/heuristic_ckc/dataset/kroB200.tsp' 200 20 9 1 200 false tsplib",
#     "bin/ckc_heuristic '/mnt/c/Users/Alex/Documents/inaoe/repos/heuristic_ckc/dataset/kroB200.tsp' 200 20 10 1 200 false tsplib",
#     "bin/ckc_heuristic '/mnt/c/Users/Alex/Documents/inaoe/repos/heuristic_ckc/dataset/kroB200.tsp' 200 40 4 1 200 false tsplib",
#     "bin/ckc_heuristic '/mnt/c/Users/Alex/Documents/inaoe/repos/heuristic_ckc/dataset/kroB200.tsp' 200 40 5 1 200 false tsplib",
#
#     # ts225
#     # PH
#     "bin/ckc_heuristic '/mnt/c/Users/Alex/Documents/inaoe/repos/heuristic_ckc/dataset/ts225.tsp' 225 5 44 100 1 false tsplib",
#     "bin/ckc_heuristic '/mnt/c/Users/Alex/Documents/inaoe/repos/heuristic_ckc/dataset/ts225.tsp' 225 5 45 100 1 false tsplib",
#     "bin/ckc_heuristic '/mnt/c/Users/Alex/Documents/inaoe/repos/heuristic_ckc/dataset/ts225.tsp' 225 10 22 100 1 false tsplib",
#     "bin/ckc_heuristic '/mnt/c/Users/Alex/Documents/inaoe/repos/heuristic_ckc/dataset/ts225.tsp' 225 10 23 100 1 false tsplib",
#     "bin/ckc_heuristic '/mnt/c/Users/Alex/Documents/inaoe/repos/heuristic_ckc/dataset/ts225.tsp' 225 20 11 100 1 false tsplib",
#     "bin/ckc_heuristic '/mnt/c/Users/Alex/Documents/inaoe/repos/heuristic_ckc/dataset/ts225.tsp' 225 20 12 100 1 false tsplib",
#     "bin/ckc_heuristic '/mnt/c/Users/Alex/Documents/inaoe/repos/heuristic_ckc/dataset/ts225.tsp' 225 40 5 100 1 false tsplib",
#     "bin/ckc_heuristic '/mnt/c/Users/Alex/Documents/inaoe/repos/heuristic_ckc/dataset/ts225.tsp' 225 40 6 100 1 false tsplib",
#     # PH+
#     "bin/ckc_heuristic '/mnt/c/Users/Alex/Documents/inaoe/repos/heuristic_ckc/dataset/ts225.tsp' 225 5 44 1 225 false tsplib",
#     "bin/ckc_heuristic '/mnt/c/Users/Alex/Documents/inaoe/repos/heuristic_ckc/dataset/ts225.tsp' 225 5 45 1 225 false tsplib",
#     "bin/ckc_heuristic '/mnt/c/Users/Alex/Documents/inaoe/repos/heuristic_ckc/dataset/ts225.tsp' 225 10 22 1 225 false tsplib",
#     "bin/ckc_heuristic '/mnt/c/Users/Alex/Documents/inaoe/repos/heuristic_ckc/dataset/ts225.tsp' 225 10 23 1 225 false tsplib",
#     "bin/ckc_heuristic '/mnt/c/Users/Alex/Documents/inaoe/repos/heuristic_ckc/dataset/ts225.tsp' 225 20 11 1 225 false tsplib",
#     "bin/ckc_heuristic '/mnt/c/Users/Alex/Documents/inaoe/repos/heuristic_ckc/dataset/ts225.tsp' 225 20 12 1 225 false tsplib",
#     "bin/ckc_heuristic '/mnt/c/Users/Alex/Documents/inaoe/repos/heuristic_ckc/dataset/ts225.tsp' 225 40 5 1 225 false tsplib",
#     "bin/ckc_heuristic '/mnt/c/Users/Alex/Documents/inaoe/repos/heuristic_ckc/dataset/ts225.tsp' 225 40 6 1 225 false tsplib"
#
#     #pr226
#     #PH
#     # "bin/ckc_heuristic '/mnt/c/Users/Alex/Documents/inaoe/repos/heuristic_ckc/dataset/pr226.tsp' 226 5 45 100 1 false tsplib",
#     # "bin/ckc_heuristic '/mnt/c/Users/Alex/Documents/inaoe/repos/heuristic_ckc/dataset/pr226.tsp' 226 5 46 100 1 false tsplib",
#     # "bin/ckc_heuristic '/mnt/c/Users/Alex/Documents/inaoe/repos/heuristic_ckc/dataset/pr226.tsp' 226 10 22 100 1 false tsplib",
#     # "bin/ckc_heuristic '/mnt/c/Users/Alex/Documents/inaoe/repos/heuristic_ckc/dataset/pr226.tsp' 226 10 23 100 1 false tsplib",
#     # "bin/ckc_heuristic '/mnt/c/Users/Alex/Documents/inaoe/repos/heuristic_ckc/dataset/pr226.tsp' 226 20 11 100 1 false tsplib",
#     # "bin/ckc_heuristic '/mnt/c/Users/Alex/Documents/inaoe/repos/heuristic_ckc/dataset/pr226.tsp' 226 20 12 100 1 false tsplib",
#     # "bin/ckc_heuristic '/mnt/c/Users/Alex/Documents/inaoe/repos/heuristic_ckc/dataset/pr226.tsp' 226 40 5 100 1 false tsplib",
#     # "bin/ckc_heuristic '/mnt/c/Users/Alex/Documents/inaoe/repos/heuristic_ckc/dataset/pr226.tsp' 226 40 6 100 1 false tsplib",
#     #PH+
#     # "bin/ckc_heuristic '/mnt/c/Users/Alex/Documents/inaoe/repos/heuristic_ckc/dataset/pr226.tsp' 226 5 45 100 226 false tsplib",
#     # "bin/ckc_heuristic '/mnt/c/Users/Alex/Documents/inaoe/repos/heuristic_ckc/dataset/pr226.tsp' 226 5 46 100 226 false tsplib",
#     # "bin/ckc_heuristic '/mnt/c/Users/Alex/Documents/inaoe/repos/heuristic_ckc/dataset/pr226.tsp' 226 10 22 100 226 false tsplib",
#     # "bin/ckc_heuristic '/mnt/c/Users/Alex/Documents/inaoe/repos/heuristic_ckc/dataset/pr226.tsp' 226 10 23 100 226 false tsplib",
#     # "bin/ckc_heuristic '/mnt/c/Users/Alex/Documents/inaoe/repos/heuristic_ckc/dataset/pr226.tsp' 226 20 11 100 226 false tsplib",
#     # "bin/ckc_heuristic '/mnt/c/Users/Alex/Documents/inaoe/repos/heuristic_ckc/dataset/pr226.tsp' 226 20 12 100 226 false tsplib",
#     # "bin/ckc_heuristic '/mnt/c/Users/Alex/Documents/inaoe/repos/heuristic_ckc/dataset/pr226.tsp' 226 40 5 100 226 false tsplib",
#     # "bin/ckc_heuristic '/mnt/c/Users/Alex/Documents/inaoe/repos/heuristic_ckc/dataset/pr226.tsp' 226 40 6 100 226 false tsplib",
#
#     #gr229
#     #PH
#     # "bin/ckc_heuristic '/mnt/c/Users/Alex/Documents/inaoe/repos/heuristic_ckc/dataset/gr229.tsp' 229 5 45 100 1 false tsplib",
#     # "bin/ckc_heuristic '/mnt/c/Users/Alex/Documents/inaoe/repos/heuristic_ckc/dataset/gr229.tsp' 229 5 46 100 1 false tsplib",
#     # "bin/ckc_heuristic '/mnt/c/Users/Alex/Documents/inaoe/repos/heuristic_ckc/dataset/gr229.tsp' 229 10 22 100 1 false tsplib",
#     # "bin/ckc_heuristic '/mnt/c/Users/Alex/Documents/inaoe/repos/heuristic_ckc/dataset/gr229.tsp' 229 10 23 100 1 false tsplib",
#     # "bin/ckc_heuristic '/mnt/c/Users/Alex/Documents/inaoe/repos/heuristic_ckc/dataset/gr229.tsp' 229 20 11 100 1 false tsplib",
#     # "bin/ckc_heuristic '/mnt/c/Users/Alex/Documents/inaoe/repos/heuristic_ckc/dataset/gr229.tsp' 229 20 12 100 1 false tsplib",
#     # "bin/ckc_heuristic '/mnt/c/Users/Alex/Documents/inaoe/repos/heuristic_ckc/dataset/gr229.tsp' 229 40 5 100 1 false tsplib",
#     # "bin/ckc_heuristic '/mnt/c/Users/Alex/Documents/inaoe/repos/heuristic_ckc/dataset/gr229.tsp' 229 40 6 100 1 false tsplib",
#     #PH+
#     # "bin/ckc_heuristic '/mnt/c/Users/Alex/Documents/inaoe/repos/heuristic_ckc/dataset/gr229.tsp' 229 5 45 100 229 false tsplib",
#     # "bin/ckc_heuristic '/mnt/c/Users/Alex/Documents/inaoe/repos/heuristic_ckc/dataset/gr229.tsp' 229 5 46 100 229 false tsplib",
#     # "bin/ckc_heuristic '/mnt/c/Users/Alex/Documents/inaoe/repos/heuristic_ckc/dataset/gr229.tsp' 229 10 22 100 229 false tsplib",
#     # "bin/ckc_heuristic '/mnt/c/Users/Alex/Documents/inaoe/repos/heuristic_ckc/dataset/gr229.tsp' 229 10 23 100 229 false tsplib",
#     # "bin/ckc_heuristic '/mnt/c/Users/Alex/Documents/inaoe/repos/heuristic_ckc/dataset/gr229.tsp' 229 20 11 100 229 false tsplib",
#     # "bin/ckc_heuristic '/mnt/c/Users/Alex/Documents/inaoe/repos/heuristic_ckc/dataset/gr229.tsp' 229 20 12 100 229 false tsplib",
#     # "bin/ckc_heuristic '/mnt/c/Users/Alex/Documents/inaoe/repos/heuristic_ckc/dataset/gr229.tsp' 229 40 5 100 229 false tsplib",
#     # "bin/ckc_heuristic '/mnt/c/Users/Alex/Documents/inaoe/repos/heuristic_ckc/dataset/gr229.tsp' 229 40 6 100 229 false tsplib",
#
#     #a280
#     #PH
#     # "bin/ckc_heuristic '/mnt/c/Users/Alex/Documents/inaoe/repos/heuristic_ckc/dataset/a280.tsp' 280 5 55 100 1 false tsplib",
#     # "bin/ckc_heuristic '/mnt/c/Users/Alex/Documents/inaoe/repos/heuristic_ckc/dataset/a280.tsp' 280 5 56 100 1 false tsplib",
#     # "bin/ckc_heuristic '/mnt/c/Users/Alex/Documents/inaoe/repos/heuristic_ckc/dataset/a280.tsp' 280 10 27 100 1 false tsplib",
#     # "bin/ckc_heuristic '/mnt/c/Users/Alex/Documents/inaoe/repos/heuristic_ckc/dataset/a280.tsp' 280 10 28 100 1 false tsplib",
#     # "bin/ckc_heuristic '/mnt/c/Users/Alex/Documents/inaoe/repos/heuristic_ckc/dataset/a280.tsp' 280 20 13 100 1 false tsplib",
#     # "bin/ckc_heuristic '/mnt/c/Users/Alex/Documents/inaoe/repos/heuristic_ckc/dataset/a280.tsp' 280 20 14 100 1 false tsplib",
#     # "bin/ckc_heuristic '/mnt/c/Users/Alex/Documents/inaoe/repos/heuristic_ckc/dataset/a280.tsp' 280 40 6 100 1 false tsplib",
#     # "bin/ckc_heuristic '/mnt/c/Users/Alex/Documents/inaoe/repos/heuristic_ckc/dataset/a280.tsp' 280 40 7 100 1 false tsplib",
#     #PH+
#     # "bin/ckc_heuristic '/mnt/c/Users/Alex/Documents/inaoe/repos/heuristic_ckc/dataset/a280.tsp' 280 5 55 100 280 false tsplib",
#     # "bin/ckc_heuristic '/mnt/c/Users/Alex/Documents/inaoe/repos/heuristic_ckc/dataset/a280.tsp' 280 5 56 100 280 false tsplib",
#     # "bin/ckc_heuristic '/mnt/c/Users/Alex/Documents/inaoe/repos/heuristic_ckc/dataset/a280.tsp' 280 10 27 100 280 false tsplib",
#     # "bin/ckc_heuristic '/mnt/c/Users/Alex/Documents/inaoe/repos/heuristic_ckc/dataset/a280.tsp' 280 10 28 100 280 false tsplib",
#     # "bin/ckc_heuristic '/mnt/c/Users/Alex/Documents/inaoe/repos/heuristic_ckc/dataset/a280.tsp' 280 20 13 100 280 false tsplib",
#     # "bin/ckc_heuristic '/mnt/c/Users/Alex/Documents/inaoe/repos/heuristic_ckc/dataset/a280.tsp' 280 20 14 100 280 false tsplib",
#     # "bin/ckc_heuristic '/mnt/c/Users/Alex/Documents/inaoe/repos/heuristic_ckc/dataset/a280.tsp' 280 40 6 100 280 false tsplib",
#     # "bin/ckc_heuristic '/mnt/c/Users/Alex/Documents/inaoe/repos/heuristic_ckc/dataset/a280.tsp' 280 40 7 100 280 false tsplib"
# ]

path = "/mnt/d/Users/ale_j/Documents/inaoe/repos/heuristic_ckc/dataset/"

# Set 2
experiments = [

    # kroA100
    # PH
    "bin/heuristic_ckc '" + path + "kroA100.tsp' 100 5 19 100 1 false tsplib",
    "bin/heuristic_ckc '" + path + "kroA100.tsp' 100 5 20 100 1 false tsplib",
    "bin/heuristic_ckc '" + path + "kroA100.tsp' 100 10 9 100 1 false tsplib",
    "bin/heuristic_ckc '" + path + "kroA100.tsp' 100 10 10 100 1 false tsplib",
    "bin/heuristic_ckc '" + path + "kroA100.tsp' 100 20 4 100 1 false tsplib",
    "bin/heuristic_ckc '" + path + "kroA100.tsp' 100 20 5 100 1 false tsplib",
    "bin/heuristic_ckc '" + path + "kroA100.tsp' 100 40 2 100 1 false tsplib",
    "bin/heuristic_ckc '" + path + "kroA100.tsp' 100 40 3 100 1 false tsplib",
    # PH+
    # "bin/heuristic_ckc '" + path + "kroA100.tsp' 100 5 19 1 100 false tsplib",
    # "bin/heuristic_ckc '" + path + "kroA100.tsp' 100 5 20 1 100 false tsplib",
    # "bin/heuristic_ckc '" + path + "kroA100.tsp' 100 10 9 1 100 false tsplib",
    # "bin/heuristic_ckc '" + path + "kroA100.tsp' 100 10 10 1 100 false tsplib",
    # "bin/heuristic_ckc '" + path + "kroA100.tsp' 100 20 4 1 100 false tsplib",
    # "bin/heuristic_ckc '" + path + "kroA100.tsp' 100 20 5 1 100 false tsplib",
    # "bin/heuristic_ckc '" + path + "kroA100.tsp' 100 40 2 1 100 false tsplib",
    # "bin/heuristic_ckc '" + path + "kroA100.tsp' 100 40 3 1 100 false tsplib",

    # kroB100
    # PH
    "bin/heuristic_ckc '" + path + "kroB100.tsp' 100 5 19 100 1 false tsplib",
    "bin/heuristic_ckc '" + path + "kroB100.tsp' 100 5 20 100 1 false tsplib",
    "bin/heuristic_ckc '" + path + "kroB100.tsp' 100 10 9 100 1 false tsplib",
    "bin/heuristic_ckc '" + path + "kroB100.tsp' 100 10 10 100 1 false tsplib",
    "bin/heuristic_ckc '" + path + "kroB100.tsp' 100 20 4 100 1 false tsplib",
    "bin/heuristic_ckc '" + path + "kroB100.tsp' 100 20 5 100 1 false tsplib",
    "bin/heuristic_ckc '" + path + "kroB100.tsp' 100 40 2 100 1 false tsplib",
    "bin/heuristic_ckc '" + path + "kroB100.tsp' 100 40 3 100 1 false tsplib",
    # PH+
    # "bin/heuristic_ckc '" + path + "kroB100.tsp' 100 5 19 1 100 false tsplib",
    # "bin/heuristic_ckc '" + path + "kroB100.tsp' 100 5 20 1 100 false tsplib",
    # "bin/heuristic_ckc '" + path + "kroB100.tsp' 100 10 9 1 100 false tsplib",
    # "bin/heuristic_ckc '" + path + "kroB100.tsp' 100 10 10 1 100 false tsplib",
    # "bin/heuristic_ckc '" + path + "kroB100.tsp' 100 20 4 1 100 false tsplib",
    # "bin/heuristic_ckc '" + path + "kroB100.tsp' 100 20 5 1 100 false tsplib",
    # "bin/heuristic_ckc '" + path + "kroB100.tsp' 100 40 2 1 100 false tsplib",
    # "bin/heuristic_ckc '" + path + "kroB100.tsp' 100 40 3 1 100 false tsplib",

    # kroC100
    # PH
    "bin/heuristic_ckc '" + path + "kroC100.tsp' 100 5 19 100 1 false tsplib",
    "bin/heuristic_ckc '" + path + "kroC100.tsp' 100 5 20 100 1 false tsplib",
    "bin/heuristic_ckc '" + path + "kroC100.tsp' 100 10 9 100 1 false tsplib",
    "bin/heuristic_ckc '" + path + "kroC100.tsp' 100 10 10 100 1 false tsplib",
    "bin/heuristic_ckc '" + path + "kroC100.tsp' 100 20 4 100 1 false tsplib",
    "bin/heuristic_ckc '" + path + "kroC100.tsp' 100 20 5 100 1 false tsplib",
    "bin/heuristic_ckc '" + path + "kroC100.tsp' 100 40 2 100 1 false tsplib",
    "bin/heuristic_ckc '" + path + "kroC100.tsp' 100 40 3 100 1 false tsplib",
    # PH+
    # "bin/heuristic_ckc '" + path + "kroC100.tsp' 100 5 19 1 100 false tsplib",
    # "bin/heuristic_ckc '" + path + "kroC100.tsp' 100 5 20 1 100 false tsplib",
    # "bin/heuristic_ckc '" + path + "kroC100.tsp' 100 10 9 1 100 false tsplib",
    # "bin/heuristic_ckc '" + path + "kroC100.tsp' 100 10 10 1 100 false tsplib",
    # "bin/heuristic_ckc '" + path + "kroC100.tsp' 100 20 4 1 100 false tsplib",
    # "bin/heuristic_ckc '" + path + "kroC100.tsp' 100 20 5 1 100 false tsplib",
    # "bin/heuristic_ckc '" + path + "kroC100.tsp' 100 40 2 1 100 false tsplib",
    # "bin/heuristic_ckc '" + path + "kroC100.tsp' 100 40 3 1 100 false tsplib",

    # eil101
    # PH
    "bin/heuristic_ckc '" + path + "eil101.tsp' 101 5 20 100 1 false tsplib",
    "bin/heuristic_ckc '" + path + "eil101.tsp' 101 5 21 100 1 false tsplib",
    "bin/heuristic_ckc '" + path + "eil101.tsp' 101 10 10 100 1 false tsplib",
    "bin/heuristic_ckc '" + path + "eil101.tsp' 101 10 11 100 1 false tsplib",
    "bin/heuristic_ckc '" + path + "eil101.tsp' 101 20 5 100 1 false tsplib",
    "bin/heuristic_ckc '" + path + "eil101.tsp' 101 20 6 100 1 false tsplib",
    "bin/heuristic_ckc '" + path + "eil101.tsp' 101 40 2 100 1 false tsplib",
    "bin/heuristic_ckc '" + path + "eil101.tsp' 101 40 3 100 1 false tsplib",
    # PH+
    # "bin/heuristic_ckc '" + path + "eil101.tsp' 101 5 20 1 101 false tsplib",
    # "bin/heuristic_ckc '" + path + "eil101.tsp' 101 5 21 1 101 false tsplib",
    # "bin/heuristic_ckc '" + path + "eil101.tsp' 101 10 10 1 101 false tsplib",
    # "bin/heuristic_ckc '" + path + "eil101.tsp' 101 10 11 1 101 false tsplib",
    # "bin/heuristic_ckc '" + path + "eil101.tsp' 101 20 5 1 101 false tsplib",
    # "bin/heuristic_ckc '" + path + "eil101.tsp' 101 20 6 1 101 false tsplib",
    # "bin/heuristic_ckc '" + path + "eil101.tsp' 101 40 2 1 101 false tsplib",
    # "bin/heuristic_ckc '" + path + "eil101.tsp' 101 40 3 1 101 false tsplib",

    # lin105
    # PH
    "bin/heuristic_ckc '" + path + "lin105.tsp' 105 5 20 100 1 false tsplib",
    "bin/heuristic_ckc '" + path + "lin105.tsp' 105 5 21 100 1 false tsplib",
    "bin/heuristic_ckc '" + path + "lin105.tsp' 105 10 10 100 1 false tsplib",
    "bin/heuristic_ckc '" + path + "lin105.tsp' 105 10 11 100 1 false tsplib",
    "bin/heuristic_ckc '" + path + "lin105.tsp' 105 20 5 100 1 false tsplib",
    "bin/heuristic_ckc '" + path + "lin105.tsp' 105 20 6 100 1 false tsplib",
    "bin/heuristic_ckc '" + path + "lin105.tsp' 105 40 2 100 1 false tsplib",
    "bin/heuristic_ckc '" + path + "lin105.tsp' 105 40 3 100 1 false tsplib",
    # PH+
    # "bin/heuristic_ckc '" + path + "lin105.tsp' 105 5 20 1 105 false tsplib",
    # "bin/heuristic_ckc '" + path + "lin105.tsp' 105 5 21 1 105 false tsplib",
    # "bin/heuristic_ckc '" + path + "lin105.tsp' 105 10 10 1 105 false tsplib",
    # "bin/heuristic_ckc '" + path + "lin105.tsp' 105 10 11 1 105 false tsplib",
    # "bin/heuristic_ckc '" + path + "lin105.tsp' 105 20 5 1 105 false tsplib",
    # "bin/heuristic_ckc '" + path + "lin105.tsp' 105 20 6 1 105 false tsplib",
    # "bin/heuristic_ckc '" + path + "lin105.tsp' 105 40 2 1 105 false tsplib",
    # "bin/heuristic_ckc '" + path + "lin105.tsp' 105 40 3 1 105 false tsplib",

    # pr107
    # PH
    "bin/heuristic_ckc '" + path + "pr107.tsp' 107 5 21 100 1 false tsplib",
    "bin/heuristic_ckc '" + path + "pr107.tsp' 107 5 22 100 1 false tsplib",
    "bin/heuristic_ckc '" + path + "pr107.tsp' 107 10 10 100 1 false tsplib",
    "bin/heuristic_ckc '" + path + "pr107.tsp' 107 10 11 100 1 false tsplib",
    "bin/heuristic_ckc '" + path + "pr107.tsp' 107 20 5 100 1 false tsplib",
    "bin/heuristic_ckc '" + path + "pr107.tsp' 107 20 6 100 1 false tsplib",
    "bin/heuristic_ckc '" + path + "pr107.tsp' 107 40 2 100 1 false tsplib",
    "bin/heuristic_ckc '" + path + "pr107.tsp' 107 40 3 100 1 false tsplib"
    # PH+
    # "bin/heuristic_ckc '" + path + "pr107.tsp' 107 5 21 1 107 false tsplib",
    # "bin/heuristic_ckc '" + path + "pr107.tsp' 107 5 22 1 107 false tsplib",
    # "bin/heuristic_ckc '" + path + "pr107.tsp' 107 10 10 1 107 false tsplib",
    # "bin/heuristic_ckc '" + path + "pr107.tsp' 107 10 11 1 107 false tsplib",
    # "bin/heuristic_ckc '" + path + "pr107.tsp' 107 20 5 1 107 false tsplib",
    # "bin/heuristic_ckc '" + path + "pr107.tsp' 107 20 6 1 107 false tsplib",
    # "bin/heuristic_ckc '" + path + "pr107.tsp' 107 40 2 1 107 false tsplib",
    # "bin/heuristic_ckc '" + path + "pr107.tsp' 107 40 3 1 107 false tsplib"
]


# Set 1
# experiments = [
#
#     # kroA200
#     # PH
#     "bin/heuristic_ckc '" + path + "kroA200.tsp' 200 5 39 100 1 false tsplib",
#     "bin/heuristic_ckc '" + path + "kroA200.tsp' 200 5 40 100 1 false tsplib",
#     "bin/heuristic_ckc '" + path + "kroA200.tsp' 200 10 19 100 1 false tsplib",
#     "bin/heuristic_ckc '" + path + "kroA200.tsp' 200 10 20 100 1 false tsplib",
#     "bin/heuristic_ckc '" + path + "kroA200.tsp' 200 20 9 100 1 false tsplib",
#     "bin/heuristic_ckc '" + path + "kroA200.tsp' 200 20 10 100 1 false tsplib",
#     "bin/heuristic_ckc '" + path + "kroA200.tsp' 200 40 4 100 1 false tsplib",
#     "bin/heuristic_ckc '" + path + "kroA200.tsp' 200 40 5 100 1 false tsplib",
#     # PH+
#     "bin/heuristic_ckc '" + path + "kroA200.tsp' 200 5 39 1 200 false tsplib",
#     "bin/heuristic_ckc '" + path + "kroA200.tsp' 200 5 40 1 200 false tsplib",
#     "bin/heuristic_ckc '" + path + "kroA200.tsp' 200 10 19 1 200 false tsplib",
#     "bin/heuristic_ckc '" + path + "kroA200.tsp' 200 10 20 1 200 false tsplib",
#     "bin/heuristic_ckc '" + path + "kroA200.tsp' 200 20 9 1 200 false tsplib",
#     "bin/heuristic_ckc '" + path + "kroA200.tsp' 200 20 10 1 200 false tsplib",
#     "bin/heuristic_ckc '" + path + "kroA200.tsp' 200 40 4 1 200 false tsplib",
#     "bin/heuristic_ckc '" + path + "kroA200.tsp' 200 40 5 1 200 false tsplib",
#
#     # kroB200
#     # PH
#     "bin/heuristic_ckc '" + path + "kroB200.tsp' 200 5 39 100 1 false tsplib",
#     "bin/heuristic_ckc '" + path + "kroB200.tsp' 200 5 40 100 1 false tsplib",
#     "bin/heuristic_ckc '" + path + "kroB200.tsp' 200 10 19 100 1 false tsplib",
#     "bin/heuristic_ckc '" + path + "kroB200.tsp' 200 10 20 100 1 false tsplib",
#     "bin/heuristic_ckc '" + path + "kroB200.tsp' 200 20 9 100 1 false tsplib",
#     "bin/heuristic_ckc '" + path + "kroB200.tsp' 200 20 10 100 1 false tsplib",
#     "bin/heuristic_ckc '" + path + "kroB200.tsp' 200 40 4 100 1 false tsplib",
#     "bin/heuristic_ckc '" + path + "kroB200.tsp' 200 40 5 100 1 false tsplib",
#     # PH+
#     "bin/heuristic_ckc '" + path + "kroB200.tsp' 200 5 39 1 200 false tsplib",
#     "bin/heuristic_ckc '" + path + "kroB200.tsp' 200 5 40 1 200 false tsplib",
#     "bin/heuristic_ckc '" + path + "kroB200.tsp' 200 10 19 1 200 false tsplib",
#     "bin/heuristic_ckc '" + path + "kroB200.tsp' 200 10 20 1 200 false tsplib",
#     "bin/heuristic_ckc '" + path + "kroB200.tsp' 200 20 9 1 200 false tsplib",
#     "bin/heuristic_ckc '" + path + "kroB200.tsp' 200 20 10 1 200 false tsplib",
#     "bin/heuristic_ckc '" + path + "kroB200.tsp' 200 40 4 1 200 false tsplib",
#     "bin/heuristic_ckc '" + path + "kroB200.tsp' 200 40 5 1 200 false tsplib",
#
#     # ts225
#     # PH
#     "bin/heuristic_ckc '" + path + "ts225.tsp' 225 5 44 100 1 false tsplib",
#     "bin/heuristic_ckc '" + path + "ts225.tsp' 225 5 45 100 1 false tsplib",
#     "bin/heuristic_ckc '" + path + "ts225.tsp' 225 10 22 100 1 false tsplib",
#     "bin/heuristic_ckc '" + path + "ts225.tsp' 225 10 23 100 1 false tsplib",
#     "bin/heuristic_ckc '" + path + "ts225.tsp' 225 20 11 100 1 false tsplib",
#     "bin/heuristic_ckc '" + path + "ts225.tsp' 225 20 12 100 1 false tsplib",
#     "bin/heuristic_ckc '" + path + "ts225.tsp' 225 40 5 100 1 false tsplib",
#     "bin/heuristic_ckc '" + path + "ts225.tsp' 225 40 6 100 1 false tsplib",
#     # PH+
#     "bin/heuristic_ckc '" + path + "ts225.tsp' 225 5 44 1 225 false tsplib",
#     "bin/heuristic_ckc '" + path + "ts225.tsp' 225 5 45 1 225 false tsplib",
#     "bin/heuristic_ckc '" + path + "ts225.tsp' 225 10 22 1 225 false tsplib",
#     "bin/heuristic_ckc '" + path + "ts225.tsp' 225 10 23 1 225 false tsplib",
#     "bin/heuristic_ckc '" + path + "ts225.tsp' 225 20 11 1 225 false tsplib",
#     "bin/heuristic_ckc '" + path + "ts225.tsp' 225 20 12 1 225 false tsplib",
#     "bin/heuristic_ckc '" + path + "ts225.tsp' 225 40 5 1 225 false tsplib",
#     "bin/heuristic_ckc '" + path + "ts225.tsp' 225 40 6 1 225 false tsplib",
#
#     # pr226
#     # PH
#     "bin/heuristic_ckc '" + path + "pr226.tsp' 226 5 45 100 1 false tsplib",
#     "bin/heuristic_ckc '" + path + "pr226.tsp' 226 5 46 100 1 false tsplib",
#     "bin/heuristic_ckc '" + path + "pr226.tsp' 226 10 22 100 1 false tsplib",
#     "bin/heuristic_ckc '" + path + "pr226.tsp' 226 10 23 100 1 false tsplib",
#     "bin/heuristic_ckc '" + path + "pr226.tsp' 226 20 11 100 1 false tsplib",
#     "bin/heuristic_ckc '" + path + "pr226.tsp' 226 20 12 100 1 false tsplib",
#     "bin/heuristic_ckc '" + path + "pr226.tsp' 226 40 5 100 1 false tsplib",
#     "bin/heuristic_ckc '" + path + "pr226.tsp' 226 40 6 100 1 false tsplib",
#     # PH+
#     "bin/heuristic_ckc '" + path + "pr226.tsp' 226 5 45 1 226 false tsplib",
#     "bin/heuristic_ckc '" + path + "pr226.tsp' 226 5 46 1 226 false tsplib",
#     "bin/heuristic_ckc '" + path + "pr226.tsp' 226 10 22 1 226 false tsplib",
#     "bin/heuristic_ckc '" + path + "pr226.tsp' 226 10 23 1 226 false tsplib",
#     "bin/heuristic_ckc '" + path + "pr226.tsp' 226 20 11 1 226 false tsplib",
#     "bin/heuristic_ckc '" + path + "pr226.tsp' 226 20 12 1 226 false tsplib",
#     "bin/heuristic_ckc '" + path + "pr226.tsp' 226 40 5 1 226 false tsplib",
#     "bin/heuristic_ckc '" + path + "pr226.tsp' 226 40 6 1 226 false tsplib",
#
#     # gr229
#     # PH
#     "bin/heuristic_ckc '" + path + "gr229.tsp' 229 5 45 100 1 false tsplib",
#     "bin/heuristic_ckc '" + path + "gr229.tsp' 229 5 46 100 1 false tsplib",
#     "bin/heuristic_ckc '" + path + "gr229.tsp' 229 10 22 100 1 false tsplib",
#     "bin/heuristic_ckc '" + path + "gr229.tsp' 229 10 23 100 1 false tsplib",
#     "bin/heuristic_ckc '" + path + "gr229.tsp' 229 20 11 100 1 false tsplib",
#     "bin/heuristic_ckc '" + path + "gr229.tsp' 229 20 12 100 1 false tsplib",
#     "bin/heuristic_ckc '" + path + "gr229.tsp' 229 40 5 100 1 false tsplib",
#     "bin/heuristic_ckc '" + path + "gr229.tsp' 229 40 6 100 1 false tsplib",
#     # PH+
#     "bin/heuristic_ckc '" + path + "gr229.tsp' 229 5 45 1 229 false tsplib",
#     "bin/heuristic_ckc '" + path + "gr229.tsp' 229 5 46 1 229 false tsplib",
#     "bin/heuristic_ckc '" + path + "gr229.tsp' 229 10 22 1 229 false tsplib",
#     "bin/heuristic_ckc '" + path + "gr229.tsp' 229 10 23 1 229 false tsplib",
#     "bin/heuristic_ckc '" + path + "gr229.tsp' 229 20 11 1 229 false tsplib",
#     "bin/heuristic_ckc '" + path + "gr229.tsp' 229 20 12 1 229 false tsplib",
#     "bin/heuristic_ckc '" + path + "gr229.tsp' 229 40 5 1 229 false tsplib",
#     "bin/heuristic_ckc '" + path + "gr229.tsp' 229 40 6 1 229 false tsplib",
#
#     # a280
#     # PH
#     "bin/heuristic_ckc '" + path + "a280.tsp' 280 5 55 100 1 false tsplib",
#     "bin/heuristic_ckc '" + path + "a280.tsp' 280 5 56 100 1 false tsplib",
#     "bin/heuristic_ckc '" + path + "a280.tsp' 280 10 27 100 1 false tsplib",
#     "bin/heuristic_ckc '" + path + "a280.tsp' 280 10 28 100 1 false tsplib",
#     "bin/heuristic_ckc '" + path + "a280.tsp' 280 20 13 100 1 false tsplib",
#     "bin/heuristic_ckc '" + path + "a280.tsp' 280 20 14 100 1 false tsplib",
#     "bin/heuristic_ckc '" + path + "a280.tsp' 280 40 6 100 1 false tsplib",
#     "bin/heuristic_ckc '" + path + "a280.tsp' 280 40 7 100 1 false tsplib",
#     # PH+
#     "bin/heuristic_ckc '" + path + "a280.tsp' 280 5 55 1 280 false tsplib",
#     "bin/heuristic_ckc '" + path + "a280.tsp' 280 5 56 1 280 false tsplib",
#     "bin/heuristic_ckc '" + path + "a280.tsp' 280 10 27 1 280 false tsplib",
#     "bin/heuristic_ckc '" + path + "a280.tsp' 280 10 28 1 280 false tsplib",
#     "bin/heuristic_ckc '" + path + "a280.tsp' 280 20 13 1 280 false tsplib",
#     "bin/heuristic_ckc '" + path + "a280.tsp' 280 20 14 1 280 false tsplib",
#     "bin/heuristic_ckc '" + path + "a280.tsp' 280 40 6 1 280 false tsplib",
#     "bin/heuristic_ckc '" + path + "a280.tsp' 280 40 7 1 280 false tsplib"
# ]

for e in experiments:
    print(e)
    os.system(e)
