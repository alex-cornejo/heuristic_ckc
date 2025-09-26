package com.jaca.masterrunner;

import java.io.BufferedWriter;
import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.util.Random;

public class TSPGenerator {
    static Random rnd = new Random();

    static void createTspInstance(String path, String name, int n) throws IOException {
        var directory = new File(path);
        if (!directory.exists()){
            if(!directory.mkdir()) throw new java.lang.Error("error when crating folder!");
        }
        final int upperBound = 5000;
        String outputFilePath = path + "/" + name;
        var file = new File(outputFilePath);
        if(file.exists()) throw new java.lang.Error("Instance cannot be overwritten!!");
        try (var writer = new BufferedWriter(new FileWriter(outputFilePath))) {
            writer.write(String.valueOf(n));
            for (int i = 1; i <= n; i++) {
                int[] xy = rnd.ints(2, 0, upperBound).toArray();
                String row = String.format("%d %d %d", i, xy[0], xy[1]);
                writer.newLine();
                writer.write(row);
            }
        }
    }


    public static void main(String[] args) throws IOException {
        String rootPath = "/your/path/to/store/instances/"; // must exist    
        int numberOfInstances = 30;
        int initSize = 100;
        int finalSize = 250;
        int offset = 50;
        // this will create instances with 100, 150, 200, 250 nodes

        final int sizeIdxInstance = 2;
        final String prefix = "URDI";

        for (int n = initSize; n <= finalSize; n += offset) {
            String pathFolder = String.format("%s/%s-%d", rootPath, prefix, n);
            for (int i = 1; i <= numberOfInstances; i++) {
                var idxInstance = new StringBuilder(Integer.toString(i));
                while (idxInstance.length() < sizeIdxInstance) {
                    idxInstance.insert(0, 0);
                }
                String instanceName = String.format("%s-%d-%s.tsp", prefix, n, idxInstance);
                createTspInstance(pathFolder, instanceName, n);
            }
        }
    }
}
