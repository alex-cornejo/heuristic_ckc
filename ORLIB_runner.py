import glob
import math
import os
import re
import subprocess
from pathlib import Path

output_path = "output_orlib.csv"
if os.path.exists(output_path):
    os.remove(output_path)
imbalance_values = [0.0, 0.05, 0.1]
dataset_path = "dataset/ORLIB/"
cores_exp = [1, 2, 4, 8, 16, 32]
# cores_exp = [1, 2]

files = glob.glob("{}/pmed*.txt".format(dataset_path))
files = sorted(files, key=lambda file: (
    int(re.findall(r'\d+', file)[0]), file))


def run_onehop(instance_path: str, L: int, np: int, ind_rep: int, rep: int):
    # create cmd
    # ./heuristic_ckc [file] [n] [k] [L] [ind_rep] [rep] [print] [instance_format]
    # with dummy "n" and "k" (they will be taken from the instance)
    cmd = "mpirun -np {} ./heuristic_ckc {} {} {} {} {} {} {} {}".format(np, instance_path, -1, -1, L, ind_rep, rep,
                                                                         "false", "orlib")
    result = subprocess.Popen(cmd, shell=True, stdout=subprocess.PIPE).stdout.read()
    out = result.decode("utf-8")
    out = out.split("\n")
    out.pop()

    return out[0]


# run all experiments
for file_path in files:

    instance_name = Path(file_path).stem

    with open(file_path) as file:
        arr_line = file.readline().strip().split(" ")
        n = int(arr_line[0])
        k = int(arr_line[2])
    prevL = -1
    for imbalance in imbalance_values:
        L = math.ceil((n - k) / k)
        L = math.ceil(round(L * (1 + imbalance), 8))

        if prevL != L:

            # try with all cores options
            line_res = "{},{},{},{}".format(instance_name, n, k, L)
            for np in cores_exp:
                res = run_onehop(file_path, L, np, 30, 1)
                line_res += ",{}".format(res)
            # print(line_res)
            with open(output_path, "a") as output_file:
                output_file.write(line_res)
                output_file.write("\n")
            prevL = L
