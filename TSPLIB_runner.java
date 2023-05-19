import java.io.*;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

public class TSPLIB_runner {

    static final int[] CORES_EXP = { 1, 2, 4, 8, 16, 32 };  // number of cores to be used in the experiments
    static final String ONEHOP_SOLVER = "./heuristic_ckc";  // compiled program of heuristic
    static final String INSTANCES_PATH = "dataset/TSPLIB";

    static List<Object[]> datasetC1 = Arrays.asList(new Object[] { "rat195", 195 }, new Object[] { "d198", 198 },
            new Object[] { "gr202", 202 }, new Object[] { "tsp225", 225 }, new Object[] { "gr229", 229 });
    static List<Object[]> datasetC2 = Arrays.asList(new Object[] { "kroA200", 200 }, new Object[] { "kroB200", 200 },
            new Object[] { "ts225", 225 }, new Object[] { "pr226", 226 }, new Object[] { "a280", 280 });

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
        int indRep = 30;    // change to 1 for OHCKC+
        int rep = 1;    // change to n for OHCKC+
        boolean printable = false;  // for masive instances, solutions are not saved
        final String solverPath = "mpirun -np " + np + " " + ONEHOP_SOLVER;
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

        if (printable) {
            StringBuilder jsonSol = new StringBuilder();
            for (int i = 1; i < lines.size(); i++) {
                jsonSol.append(lines.get(i));
            }
            String pathTSPSol = instancePath.replaceFirst("tsplib", "solutions/tsplib");
            pathTSPSol = pathTSPSol.replace(".tsp", String.format("-%d-%d.json", k, L));
            File fileTSPSol = new File(pathTSPSol);
            File folderTSPSol = new File(fileTSPSol.getParent());
            if (!folderTSPSol.exists())
                folderTSPSol.mkdirs();
            try (BufferedWriter writer = new BufferedWriter(new FileWriter(fileTSPSol))) {
                writer.write(jsonSol.toString());
            }
        }

        return new double[] { fitnessCVKP, avgFitness, stdDev, time, timeParallelSec, card };
    }

    public static void main(String[] args) throws IOException, InterruptedException {

        int[] K = { 5, 10, 20, 40 };
        double[] margins = { 0, 0.05, 0.1 };
        int numberOfInstances = 1;

        BufferedWriter outputWriter = new BufferedWriter(new FileWriter("TSPLIB_output.csv"));
        for (Object[] pair : datasetC1) {   // change to datasetC2 for dataset C2
            int n = (int) pair[1];
            for (int k : K) {
                int prevL = -1;
                for (double margin : margins) {
                    int L = (int) Math.ceil((float) (n - k) / k);
                    L = (int) Math.ceil((float) (L * (1 + margin)));

                    if (prevL != L) {

                        for (int i = 1; i <= numberOfInstances; i++) {

                            String tspInstanceName = String.format("%s.tsp", pair[0]);
                            String tspInstancePath = String.format("%s/%s", INSTANCES_PATH,
                                    tspInstanceName);

                            // System.out.printf("%s, %d, %d, %d\n", tspInstanceName, n, k, L);
                            StringBuilder line = new StringBuilder();
                            for (int np : CORES_EXP) {
                                double[] objValuesOneHop = executeOneHopExperiment(tspInstancePath, n, k, L, np);
                                int objCvkpOH = (int) objValuesOneHop[0];
                                double avgFitness = objValuesOneHop[1];
                                double stdDev = objValuesOneHop[2];
                                double time = objValuesOneHop[3];
                                double timeParallelSec = objValuesOneHop[4];
                                int card = (int) objValuesOneHop[5];
                                line.append(String.format(",%d,%f,%f,%f,%f,%d", objCvkpOH, avgFitness, stdDev, time,
                                        timeParallelSec, card));
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
        System.out.println("\n\nfinished");
    }
}
