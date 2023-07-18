import math
import os
import pandas as pd

coresval = [1, 2, 4, 8]  # number of cores to be used in the experiments
kvals = [5, 10, 20, 40]
# nvals = [1000]
nvals = [1000, 2000, 3000, 4000, 5000]
margins = [0, 0.05, 0.1]

# Load the data
data = pd.read_csv("Table4_fulloutput.csv", sep=",")

figures_dir = "Table4_figures/"
if not os.path.exists(figures_dir):
    os.makedirs(figures_dir)

output_writer = open("Table4_output.csv", "w")
header = "instance,n,k,L,BKS,"
for cores in coresval:
    if cores != 1:
        header += "S{},".format(cores)

print(header)
output_writer.write(header + "\n")

for n in nvals:

    dataplot = {}

    for k in kvals:

        for margin in margins:

            L = int(math.ceil((n-k)/k))
            L = int(math.ceil((L * (1 + margin))))
            instance_name = "URDI-{}-01.tsp".format(n)

            tmpdf = data.query('instance=="{}" & k=={} & L=={}'.format(
                instance_name, k, L), engine="python")
            times = []
            speedups = []
            line = "{},{},{},{},".format(instance_name, n, k, L)
            for cores in coresval:
                if cores == 1:
                    best1 = tmpdf.iloc[0]["best{}".format(cores)]
                    timepar1 = tmpdf.iloc[0]["timepar{}".format(cores)]
                    line += "{},".format(best1)

                timepar = tmpdf.iloc[0]["timepar{}".format(cores)]
                times.append(timepar)
                S = timepar1/timepar
                speedups.append(S)

                if cores > 1:
                    line += "{},".format(S)
            print(line)
            output_writer.write(line + "\n")

            if margin==0:
                dataplot[k] = {"times": times, "speedups": speedups}

    # Plot the speedups
    import matplotlib.pyplot as plt
    plt.clf()
    plt.plot(coresval, dataplot[40]["speedups"], 'ro-', color='black')
    plt.plot(coresval, dataplot[20]["speedups"], 'ro-', color='green')
    plt.plot(coresval, dataplot[10]["speedups"], 'ro-', color='red')
    plt.plot(coresval, dataplot[5]["speedups"], 'ro-', color='blue')
    plt.grid(True)
    plt.xlabel('number of cores')
    plt.ylabel('speedup')
    plt.legend(('k=40', 'k=20', 'k=10', 'k=5'),
               loc='upper center', shadow=False)
    # plt.show()
    plt.savefig(figures_dir + "{}-speedup.png".format(instance_name))

    # plot the times
    plt.clf()
    plt.plot(coresval, dataplot[40]["times"], 'ro-', color='black')
    plt.plot(coresval, dataplot[20]["times"], 'ro-', color='green')
    plt.plot(coresval, dataplot[10]["times"], 'ro-', color='red')
    plt.plot(coresval, dataplot[5]["times"], 'ro-', color='blue')
    plt.xlabel('number of cores')
    plt.grid(True)
    plt.ylabel('seconds')
    plt.legend(('k=40', 'k=20', 'k=10', 'k=5'),
               loc='upper center', shadow=False)
    # plt.show()
    plt.savefig(figures_dir + "{}-times.png".format(instance_name))


output_writer.close()
