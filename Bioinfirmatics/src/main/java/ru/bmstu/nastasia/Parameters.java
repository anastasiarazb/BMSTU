package ru.bmstu.nastasia;

import com.google.gson.Gson;
import com.google.gson.GsonBuilder;
import com.google.gson.annotations.SerializedName;

import ru.bmstu.nastasia.comparator.BLOSUM;
import ru.bmstu.nastasia.comparator.CharComparator;
import ru.bmstu.nastasia.comparator.NkMatrix;

import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Paths;

public class Parameters {

    // NW - классический, NWA - с аффинным штрафом, MSA - множественное сравнение, SW - SmithWaterman
    private String algorithm     = "NW";
    @SerializedName("fasta source")
    private String fastaSource = "test.fasta"; // Путь к папке с исходными последовательностями
    @SerializedName("match reward")
    private int matchReward =  4;   // m, награда за совпадение
    @SerializedName("mismatch penalty")
    private int mismatchPenalty =  2;   // s, штраф за несовпадение
    @SerializedName("gap penalty")
    private float gapPenalty = -2;   // d, штраф за разрыв (за открытие делеции)
    @SerializedName("extension gap penalty")
    private float extGapPenalty = -1;   // e, штраф за удлинение делеции
    @SerializedName("output line length")
    private int outputLineLength = 20;
    @SerializedName("compare function")
    private String compareFunction = "CMP"; //Использовать матрицы BLOSUM, NK / Обычное сравнение CMP
    @SerializedName("k band")
    private int kBand = 5; // Путь к папке с исходными последовательностями

    public static Parameters extract(String path) {
        Gson g = new Gson();
        try {
            byte[] encoded = Files.readAllBytes(Paths.get(path));
            String content = new String(encoded);
            Parameters prop = g.fromJson(content, Parameters.class);
            if (prop == null) {
                System.out.println("Unable to read parameters from file, set default values. See README for parameters ");
                return new Parameters();
            }
            return prop;
        } catch (IOException ex) {
            System.out.println("Preferences file was not specified. Nothing to extract.");
            ex.printStackTrace();
        }
        return new Parameters();
    }

    public CharComparator getSimpleComparator() {
        return new CharComparator() {
            @Override
            public float compare(char a, char b) {
                return (a == b)
                        ? matchReward
                        : mismatchPenalty;
            }
        };
    }

    public static Parameters parseParams(String[] args) {
        if (args.length < 1) {
            System.out.println("Usage: <prog_name> <json_param_file>\nDefault parameters used.");
        }
        return extract(args[0]);
    }

    public CharComparator getComparator() {
        String func = compareFunction.toUpperCase();
        if (func.equals("CMP")) {
            return getSimpleComparator();
        }

        if (func.equals("B") || func.equals("BLOSUM")) {
            return BLOSUM.comparator;
        }

        if (func .equals("NK")) {
            return NkMatrix.comparator;
        }

        System.out.println("Sorry, compare function \"" + compareFunction + "\" is not found.\n " +
                "Expected: \"CMP\", \"NK\", \"BLOSUM\".\n" +
                "Default simple comparator \"CMP\" used.");
        return getSimpleComparator();
    }

    // -------------------------- Getters ----------------------------------
    public String getAlgorithm() {
        return algorithm;
    }

    public String getFastaSource() {
        return fastaSource;
    }

    public int getMatchReward() {
        return matchReward;
    }

    public int getMismatchPenalty() {
        return mismatchPenalty;
    }

    public float getGapPenalty() {
        return gapPenalty;
    }

    public float getExtGapPenalty() {
        return extGapPenalty;
    }

    public int getOutputLineLength() {
        return outputLineLength;
    }

    public String getCompareFunction() {
        return compareFunction;
    }

    public int getKBand() {
        return kBand;
    }

    @Override
    public String toString() {
        return new GsonBuilder().setPrettyPrinting().create().toJson(this);
    }
}
