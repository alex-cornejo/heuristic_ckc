
import org.apache.commons.math3.stat.inference.MannWhitneyUTest;

import java.io.*;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

// run this file from the root of the project
// java -cp commons-math3-3.5.jar:. Table3_runner.java

public class Table3_runner {
    // paths of compiled files of PDISP heuristic
    static final String PDISPSolverPath = "/home/alex/Documents/inaoe/repos/pdisp_ckc";

    static List<String> executeCmd(String cmd) throws IOException, InterruptedException {
        Runtime run = Runtime.getRuntime();
        Process pr = run.exec(cmd);
        pr.waitFor();
        List<String> lines = new ArrayList<>();
        try (var buf = new BufferedReader(new InputStreamReader(pr.getInputStream()))) {
            String line;
            while ((line = buf.readLine()) != null) {
                lines.add(line);
            }
        }
        return lines;
    }

    static int executePDispExperiment(String instancePath) throws IOException, InterruptedException {
        String cmd = String.format("%s/pdisp_ckc %s 0 0 0 output.txt 1", PDISPSolverPath, instancePath);
        List<String> lines = executeCmd(cmd);

        String[] firstRowArr = lines.get(0).split(",");
        int fitnessCVKP = Integer.parseInt(firstRowArr[0]);
        return fitnessCVKP;
    }

    static int executeOneHopExperiment(String instancePath, int n, int k, int L)
            throws IOException, InterruptedException {
        boolean printable = false;
        String cmd = String.format("./heuristic_ckc %s %d %d %d 1 1 %b tsplib", instancePath, n, k, L,
                printable);
        List<String> lines = executeCmd(cmd);

        String[] firstRowArr = lines.get(0).split(",");
        int fitnessCVKP = Integer.parseInt(firstRowArr[0]);
        return fitnessCVKP;
    }

    static double wilcoxonTest(ArrayList<Integer> x, ArrayList<Integer> y) {
        MannWhitneyUTest test = new MannWhitneyUTest();
        double p = test.mannWhitneyUTest(
                x.stream().mapToDouble(i -> i).toArray(),
                y.stream().mapToDouble(i -> i).toArray());
        return p;
    }

    static String exactCvkpResults = "dataset/URDI/optimalsolutions.txt";

    public static void main(String[] args) throws IOException, InterruptedException {

        int[] K = { 5, 10, 20, 40 };
        double[] margins = { 0, 0.05, 0.1 };
        int numberOfInstances = 30;
        int initSize = 100;
        int finalSize = 250;
        int offset = 50;
        final int sizeIdxInstance = 2;

        var outputWriter = new BufferedWriter(new FileWriter("Table3_output.csv"));
        var exactCvkpResultsReader = new BufferedReader(new FileReader(exactCvkpResults));
        for (int n = initSize; n <= finalSize; n += offset) {

            for (int k : K) {
                int prevL = -1;
                for (double margin : margins) {

                    int L = (int) Math.ceil((float) (n - k) / k);
                    L = (int) Math.ceil((float) (L * (1 + margin)));
                    if (prevL != L) {

                        var oneHopCVKP = new ArrayList<Integer>();
                        var pDispCVKP = new ArrayList<Integer>();

                        int oneHopChecks = 0;
                        int pdispChecks = 0;
                        double oneHopAF = 0;
                        double pdispAF = 0;
                        for (int i = 1; i <= numberOfInstances; i++) {
                            var idxInstance = new StringBuilder(Integer.toString(i));
                            while (idxInstance.length() < sizeIdxInstance) {
                                idxInstance.insert(0, 0);
                            }
                            String OHCKCInstanceName = String.format("URDI-%d-%s.tsp", n, idxInstance);
                            String PDISPInstanceName = String.format("URDI-%d-%s-%d-%d.cvkp", n, idxInstance, k, L + 1);

                            String OHCKCInstancePath = String.format("dataset/URDI/URDI-%d/%s", n, OHCKCInstanceName);
                            String PDISPInstancePath = String.format("%s/dataset/URDI-%d/%s", PDISPSolverPath, n,
                                    PDISPInstanceName);

                            int objValuesOneHop = executeOneHopExperiment(OHCKCInstancePath, n, k, L);
                            int objValuesPDisp = executePDispExperiment(PDISPInstancePath);

                            int objCvkpOH = objValuesOneHop;
                            oneHopCVKP.add(objValuesOneHop);

                            int objCvkpPD = objValuesPDisp;
                            pDispCVKP.add(objValuesPDisp);

                            if (objCvkpOH > objCvkpPD)
                                pdispChecks++;
                            if (objCvkpOH < objCvkpPD)
                                oneHopChecks++;

                            // read exact results of CVKP
                            String[] exactResultArr = exactCvkpResultsReader.readLine().split(", ");
                            // System.out.println(Arrays.toString(exactResultArr));
                            if (Integer.parseInt(exactResultArr[1]) != k
                                    || Integer.parseInt(exactResultArr[2]) != (L + 1)) {
                                throw new java.lang.Error("Error in exact results");
                            }
                            double optCvkp = Double.parseDouble(exactResultArr[3]);
                            oneHopAF += objCvkpOH / optCvkp;
                            pdispAF += objCvkpPD / optCvkp;
                            exactCvkpResultsReader.readLine(); // skip json result
                        }

                        oneHopAF /= numberOfInstances;
                        pdispAF /= numberOfInstances;
                        var pvalue = wilcoxonTest(oneHopCVKP, pDispCVKP);
                        String line = String.format("URDI-%d,%d,%d,%d,%f,%d,%f,%f,%d,%f,%f,%d,%d",
                                n,
                                n,
                                k,
                                L,
                                oneHopCVKP.stream().mapToDouble(v -> v).average().getAsDouble(),
                                oneHopChecks,
                                oneHopAF,
                                pDispCVKP.stream().mapToDouble(v -> v).average().getAsDouble(),
                                pdispChecks,
                                pdispAF,
                                pvalue,
                                pvalue<0.05?1:0,
                                pvalue<0.01?1:0);

                        System.out.println(line);
                        outputWriter.write(String.format("%s\n", line));
                        prevL = L;
                    }
                }
            }
        }
        outputWriter.close();
        exactCvkpResultsReader.close();
        System.out.println("\n\nfinished");
    }
}
