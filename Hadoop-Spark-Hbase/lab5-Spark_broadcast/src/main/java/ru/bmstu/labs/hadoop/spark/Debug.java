package ru.bmstu.labs.hadoop.spark;

/**
 * Created by nastasia on 08.12.16.
 */
public class Debug {
    private static boolean debugMode = false;

    public static void out(String s) {
        if (debugMode) {
            System.out.println("\n****** " + s  + "\n");
        }
    }

    public static void set(boolean statement) {
        debugMode = statement;
    }
}