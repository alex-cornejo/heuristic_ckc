import java.io.*;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

public class Table6_runner {

    static final String ONEHOP_SOLVER = "./heuristic_ckc"; // compiled program of heuristic
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

    static double[] searchExactSolution(String instance, int k, int L) {
        try {
            var reader = new BufferedReader(new FileReader("dataset/TSPLIB/optimalsolutions.txt"));
            String line = reader.readLine();

            while (line != null) {
                String[] arrLine = line.strip().replace(" ", "").split(",");
                int thisK = Integer.parseInt(arrLine[1]);
                int thisL = Integer.parseInt(arrLine[2]);
                double objVal = Double.parseDouble(arrLine[3]);
                double time = Double.parseDouble(arrLine[5]);
                if (instance.startsWith(arrLine[0]) && k == thisK && L == thisL) {
                    reader.close();
                    return new double[] { objVal, time };
                }
                reader.readLine();  // skip json result
                line = reader.readLine();
            }
            reader.close();

        } catch (Exception e) {
            e.printStackTrace();
        }
        return null;
    }

    static double[] executeOneHopExperiment(String instancePath, int n, int k, int L, int np, int indRep, int rep)
            throws IOException, InterruptedException {
        // int indRep = 30; // change to 1 for OHCKC+
        // int rep = 1; // change to n for OHCKC+
        boolean printable = false; // for masive instances, solutions are not saved
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

        return new double[] { fitnessCVKP, avgFitness, stdDev, time, timeParallelSec, card };
    }

    public static void main(String[] args) throws IOException, InterruptedException {

        int[] K = { 5, 10, 20, 40 };
        double[] margins = { 0, 0.05, 0.1 };
        int numberOfInstances = 1;

        BufferedWriter outputWriter = new BufferedWriter(new FileWriter("Table6_output.csv"));
        outputWriter.write(
                "instance,n,k,L,OPT,time,avgOHCKC8,stdOHCKC8,timeparOHCKC8,S,gap,avgOHCKCp8,timeparOHCKCp8,S,gap\n");
        System.out.println(
                "instance,n,k,L,OPT,time,avgOHCKC8,stdOHCKC8,timeparOHCKC8,S,gap,avgOHCKCp8,timeparOHCKCp8,S,gap");

        for (Object[] pair : datasetC2) { // change to datasetC2 for dataset C2
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

                            var optsol = searchExactSolution(tspInstanceName, k, L);
                            if(optsol == null) {
                                throw new java.lang.Error("Error in exact results");
                            }
                            double opt = optsol[0];
                            double time = optsol[1];

                            // System.out.printf("%s, %d, %d, %d\n", tspInstanceName, n, k, L);
                            StringBuilder line = new StringBuilder();
                            line.append(String.format("%s,%d,%d,%d,%f,%f,", tspInstanceName, n, k, L, opt, time));

                            // OHCKC 1 core
                            double[] ansOHCKC = executeOneHopExperiment(tspInstancePath, n, k, L, 1, 30, 1);
                            int bestOHCKC = (int) ansOHCKC[0];
                            double avgOHCKC = ansOHCKC[1];
                            double stdOHCKC = ansOHCKC[2];
                            double timeOHCKC = ansOHCKC[3];
                            double timeparOHCKC = ansOHCKC[4];
                            int cardOHCKC = (int) ansOHCKC[5];

                            // OHCKC 8 cores
                            double[] ansOHCKC8 = executeOneHopExperiment(tspInstancePath, n, k, L, 8, 30, 1);
                            int bestOHCKC8 = (int) ansOHCKC8[0];
                            double avgOHCKC8 = ansOHCKC8[1];
                            double stdOHCKC8 = ansOHCKC8[2];
                            double timeOHCKC8 = ansOHCKC8[3];
                            double timeparOHCKC8 = ansOHCKC8[4];
                            int cardOHCKC8 = (int) ansOHCKC8[5];

                            double S = timeparOHCKC / timeparOHCKC8;
                            line.append(String.format("%f,%f,%f,%f,%f,", avgOHCKC8, stdOHCKC8, timeparOHCKC8, S, (avgOHCKC8/opt)-1));

                            // OHCKC+ 1 core
                            double[] ansOHCKCp = executeOneHopExperiment(tspInstancePath, n, k, L, 1, 1, n);
                            int bestOHCKCp = (int) ansOHCKCp[0];
                            double avgOHCKCp = ansOHCKCp[1];
                            double stdOHCKCp = ansOHCKCp[2];
                            double timeOHCKCp = ansOHCKCp[3];
                            double timeparOHCKCp = ansOHCKCp[4];
                            int cardOHCKCp = (int) ansOHCKCp[5];

                            // OHCKC+ 8 cores
                            double[] ansOHCKCp8 = executeOneHopExperiment(tspInstancePath, n, k, L, 8, 1, n);
                            int bestOHCKCp8 = (int) ansOHCKCp8[0];
                            double avgOHCKCp8 = ansOHCKCp8[1];
                            double stdOHCKCp8 = ansOHCKCp8[2];
                            double timeOHCKCp8 = ansOHCKCp8[3];
                            double timeparOHCKCp8 = ansOHCKCp8[4];
                            int cardOHCKCp8 = (int) ansOHCKCp8[5];

                            S = timeparOHCKCp / timeparOHCKCp8;
                            line.append(String.format("%f,%f,%f,%f", avgOHCKCp8, timeparOHCKCp8, S, (avgOHCKCp8/opt)-1));

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
