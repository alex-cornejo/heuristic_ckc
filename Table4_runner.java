import java.io.*;
import java.util.ArrayList;
import java.util.List;

public class Table4_runner {

    static final int[] CORES_EXP = { 1, 2, 4, 8 }; // number of cores to be used in the experiments

    static List<String> executeCmd(String cmd) throws IOException, InterruptedException {
        Runtime run = Runtime.getRuntime();
        Process pr = run.exec(cmd);
        pr.waitFor();
        List<String> lines = new ArrayList<>();
        try (BufferedReader buf = new BufferedReader(new InputStreamReader(pr.getInputStream()))) {
            String line;
            while ((line = buf.readLine()) != null) {
                lines.add(line);
            }
        }
        return lines;
    }

    static double[] executeOneHopExperiment(String instancePath, int n, int k, int L, int np)
            throws IOException, InterruptedException {
        int indRep = 30;
        int rep = 1;
        boolean printable = false;
        final String solverPath = "mpirun -np " + np + " heuristic_ckc";
        String cmd = String.format("%s %s %d %d %d %d %d %b tsplib", solverPath, instancePath, n, k, L, indRep, rep,
                printable);
        List<String> lines = executeCmd(cmd);
        String[] firstRowArr = lines.get(0).split(",");
        int fitnessCVKP = Integer.parseInt(firstRowArr[0]);
        double avgFitness = Double.parseDouble(firstRowArr[1]);
        double stdDev = Double.parseDouble(firstRowArr[2]);
        double time = Double.parseDouble(firstRowArr[3]);
        double timeParallelSec = Double.parseDouble(firstRowArr[4]);
        int card = Integer.parseInt(firstRowArr[5]);

        return new double[] { fitnessCVKP, avgFitness, stdDev, time, timeParallelSec, card };
    }

    public static void main(String[] args) throws IOException, InterruptedException {
        int[] K = { 5, 10, 20, 40 };
        double[] margins = { 0, 0.05, 0.1 };
        int numberOfInstances = 1; // number of instances per size (e.g., URDI-100-01.tsp, URDI-100-02.tsp, etc.)
        int initSize = 1000;
        int finalSize = 5000;
        int offset = 1000; // offset for instances, e.g., URDI-1000-01.tsp, URDI-2000-01.tsp, etc.
        final int sizeIdxInstance = 2;

        BufferedWriter outputWriter = new BufferedWriter(new FileWriter("Table4_fulloutput.csv"));
        StringBuilder line = new StringBuilder();
        line.append("instance,n,k,L,");
        for (int np : CORES_EXP) {
            line.append(String.format("best%d,avg%d,time%d,timepar%d,card%d,", np, np, np, np, np));
        }
        System.out.println(line);
        outputWriter.write(String.format("%s\n", line));
        for (int n = initSize; n <= finalSize; n += offset) {

            for (int k : K) {
                int prevL = -1;
                for (double margin : margins) {
                    int L = (int) Math.ceil((float) (n - k) / k);
                    L = (int) Math.ceil((float) (L * (1 + margin)));
                    if (prevL != L) {

                        for (int i = 1; i <= numberOfInstances; i++) {
                            StringBuilder idxInstance = new StringBuilder(Integer.toString(i));
                            while (idxInstance.length() < sizeIdxInstance) {
                                idxInstance.insert(0, 0);
                            }
                            String OHCKCInstanceName = String.format("URDI-%d-%s.tsp", n, idxInstance);
                            String OHCKCInstancePath = String.format("dataset/URDI/URDI-%d/%s", n, OHCKCInstanceName);

                            // System.out.printf("%s, %d, %d, %d\n", tspInstanceName, n, k , L);
                            line = new StringBuilder();
                            line.append(String.format("%s,%d,%d,%d,", OHCKCInstanceName, n, k, L));
                            for (int np : CORES_EXP) {
                                double[] objValuesOneHop = executeOneHopExperiment(OHCKCInstancePath, n, k, L, np);
                                int best = (int) objValuesOneHop[0];
                                double avg = objValuesOneHop[1];
                                double std = objValuesOneHop[2];
                                double time = objValuesOneHop[3];
                                double timepar = objValuesOneHop[4];
                                int card = (int) objValuesOneHop[5];
                                line.append(String.format("%d,%f,%f,%f,%d,",best, avg, time,timepar, card));
                            }
                            System.out.println(line);
                            outputWriter.write(String.format("%s\n", line));
                        }
                        prevL = L;
                    }
                }
            }
        }
        outputWriter.close();
        executeCmd("python3 Table4_figures_speedups.py");
        System.out.println("\n\nfinished");
    }
}
