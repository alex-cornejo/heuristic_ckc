import glob
import math
import os
import re
import subprocess
from pathlib import Path
import pandas as pd

output_path = "Tables7-8_output.csv"    # output file to save results of the experiments
if os.path.exists(output_path):
    os.remove(output_path)
imbalance_values = [0.0, 0.05, 0.1]
dataset_path = "dataset/ORLIB/"

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

# Load the data
data = pd.read_csv("dataset/ORLIB/optimalsolutions.csv")

# header in output file
with open(output_path, "a") as output_file:
    output_file.write("instance,n,k,L,opt,timeinc,avgOHCKC8,stdOHCKC8,timeparOHCKC8,S8,gap,avgOHCKCp8,timeparOHCKCp8,S8,gap\n")

print("instance,n,k,L,opt,timeinc,avgOHCKC8,stdOHCKC8,timeparOHCKC8,S8,gap,avgOHCKCp8,timeparOHCKCp8,S8,gap")
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

            # optimal solution data
            tmpdf = data.query('instance=="{}.txt" & k=={} & L=={}'.format(
                    instance_name, k, L), engine="python")
        
            opt = "-"
            timeinc = "-"
            if len(tmpdf)> 0:
                opt = tmpdf.iloc[0]["OPT"]
                timeinc = tmpdf.iloc[0]["timeinc"]
            

            line = "{},{},{},{},{},{},".format(instance_name, n, k, L, opt, timeinc)
            
            # OHCKC 1 core
            ansOHCKC = run_onehop(file_path, L, 1, 30, 1)
            ansOHCKC = ansOHCKC.split(",")

            bestOHCKC = int( ansOHCKC[0])
            avgOHCKC = float(ansOHCKC[1])
            stdOHCKC = float(ansOHCKC[2])
            timeOHCKC = float(ansOHCKC[3])
            timeparOHCKC = float(ansOHCKC[4])
            cardOHCKC = int(ansOHCKC[5])

            # OHCKC 8 cores
            ansOHCKC8 = run_onehop(file_path, L, 8, 30, 1)
            ansOHCKC8 = ansOHCKC8.split(",")
            bestOHCKC8 = int(ansOHCKC8[0])
            avgOHCKC8 = float(ansOHCKC8[1])
            stdOHCKC8 = float(ansOHCKC8[2])
            timeOHCKC8 = float(ansOHCKC8[3])
            timeparOHCKC8 = float(ansOHCKC8[4])
            cardOHCKC8 = int(ansOHCKC8[5])

            S = timeparOHCKC / timeparOHCKC8
            if opt != "-":
                line+="{},{},{},{},{},".format(avgOHCKC8, stdOHCKC8, timeparOHCKC8, S, (avgOHCKC8/opt)-1)
            else:
                line+="{},{},{},{},{},".format(avgOHCKC8, stdOHCKC8, timeparOHCKC8, S, "-")

            # OHCKC+ 1 core
            ansOHCKCp = run_onehop(file_path, L, 1, 1, n)
            ansOHCKCp = ansOHCKCp.split(",")
            bestOHCKCp = int(ansOHCKCp[0])
            avgOHCKCp = float(ansOHCKCp[1])
            stdOHCKCp = float(ansOHCKCp[2])
            timeOHCKCp = float(ansOHCKCp[3])
            timeparOHCKCp = float(ansOHCKCp[4])
            cardOHCKCp = int(ansOHCKCp[5])

            # OHCKC+ 8 cores
            ansOHCKCp8 = run_onehop(file_path, L, 8, 1, n)
            ansOHCKCp8 = ansOHCKCp8.split(",")
            bestOHCKCp8 = int(ansOHCKCp8[0])
            avgOHCKCp8 = float(ansOHCKCp8[1])
            stdOHCKCp8 = float(ansOHCKCp8[2])
            timeOHCKCp8 = float(ansOHCKCp8[3])
            timeparOHCKCp8 = float(ansOHCKCp8[4])
            cardOHCKCp8 = int(ansOHCKCp8[5])

            S = timeparOHCKCp / timeparOHCKCp8

            if opt != "-":
                line+="{},{},{},{},".format(avgOHCKCp8, timeparOHCKCp8, S, (avgOHCKCp8/opt)-1)
            else:
                line+="{},{},{},{},".format(avgOHCKCp8, timeparOHCKCp8, S, "-")

            print(line)
            with open(output_path, "a") as output_file:
                output_file.write(line)
                output_file.write("\n")
            prevL = L
