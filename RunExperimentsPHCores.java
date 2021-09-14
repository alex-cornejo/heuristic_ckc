import java.io.*;
import java.util.ArrayList;
import java.util.List;

public class RunExperimentsPHCores
{

    static final String tspFolderName = "tsplib";
    static final int[] CORES_EXP = {1, 2, 4, 8, 16, 32, 48, 64};
    static final String ONEHOP_SOLVER = "./heuristic_ckc";
    static final String INSTANCES_PATH = "dataset";

    static List<String> executeCmd(String cmd) throws IOException, InterruptedException
    {
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

    static double[] executeOneHopExperiment(String instancePath, int n, int k, int L, int np) throws IOException, InterruptedException
    {
        int indRep = 30;
        int rep = 1;
        boolean printable = false;
        final String solverPath = "mpirun -np " + np + " "+ ONEHOP_SOLVER;
        String cmd = String.format("%s %s %d %d %d %d %d %b tsplib", solverPath, instancePath, n, k, L, indRep, rep, printable);
        List<String> lines = executeCmd(cmd);
        String[] firstRowArr = lines.get(0).split(",");
        int fitnessCVKP = Integer.parseInt(firstRowArr[0]);
        double avgFitness = Double.parseDouble(firstRowArr[1]);
        double stdDev = Double.parseDouble(firstRowArr[2]);
        double time = Double.parseDouble(firstRowArr[3]);

        if (printable) {
            StringBuilder jsonSol = new StringBuilder();
            for (int i = 1; i < lines.size(); i++) {
                jsonSol.append(lines.get(i));
            }
            String pathTSPSol = instancePath.replaceFirst("tsplib", "solutions/tsplib");
            pathTSPSol = pathTSPSol.replace(".tsp", String.format("-%d-%d.json", k, L));
            File fileTSPSol = new File(pathTSPSol);
            File folderTSPSol = new File(fileTSPSol.getParent());
            if (!folderTSPSol.exists()) folderTSPSol.mkdirs();
            try (BufferedWriter writer = new BufferedWriter(new FileWriter(fileTSPSol))) {
                writer.write(jsonSol.toString());
            }
        }

        return new double[]{fitnessCVKP, avgFitness, stdDev,time};
    }

    public static void main(String[] args) throws IOException, InterruptedException
    {
        int[] K = {5, 10, 20, 40};
        double[] margins = {0, 0.05, 0.1};
        int numberOfInstances = 1;
        int initSize = 1000;
        int finalSize = 5000;
        int offset = 1000;
        final int sizeIdxInstance = 2;
        final String prefix = "URDI";

        BufferedWriter outputWriter = new BufferedWriter(new FileWriter("output.csv"));
        for (int n = initSize; n <= finalSize; n += offset) {

            for (int k : K) {
                int prevL = -1;
                for (double margin : margins) {
                    int L = (int) Math.ceil((float) n / k);
                    L = (int) Math.ceil((float) (L * (1 + margin)));
                    L--;
                    if (prevL != L) {

                        for (int i = 1; i <= numberOfInstances; i++) {
                            StringBuilder idxInstance = new StringBuilder(Integer.toString(i));
                            while (idxInstance.length() < sizeIdxInstance) {
                                idxInstance.insert(0, 0);
                            }
                            String tspInstanceName = String.format("%s-%d-%s.tsp", prefix, n, idxInstance);
                            String tspInstancePath = String.format("%s/%s/%s-%d/%s",
                                INSTANCES_PATH, tspFolderName, prefix, n, tspInstanceName);

//                            System.out.printf("%s, %d, %d, %d\n", tspInstanceName, n, k , L);
                            StringBuilder line = new StringBuilder();
                            for (int np : CORES_EXP) {
                                double[] objValuesOneHop = executeOneHopExperiment(tspInstancePath, n, k, L, np);
                                int objCvkpOH = (int) objValuesOneHop[0];
                                double avgFitness = objValuesOneHop[1];
                                double stdDev = objValuesOneHop[2];
                                double time = objValuesOneHop[3];
                                line.append(String.format(",%d,%f,%f", objCvkpOH, avgFitness,time));
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
