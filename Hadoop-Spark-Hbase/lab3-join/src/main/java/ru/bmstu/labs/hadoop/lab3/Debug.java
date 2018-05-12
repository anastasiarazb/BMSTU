package ru.bmstu.labs.hadoop.lab3;

/**
 * Created by nastasia on 12.11.16.
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