package ru.bmstu.labs.hadoop.spark;

import java.io.Serializable;

/**
 * Created by nastasia on 08.12.16.
 */

//Аккумулятор

public class DelayStatistics implements Serializable {
    private int   recordsNum;
    private float maxDelay;
    private int   numDelayed;
    private int   numCancelled;

    public int getRecordsNum() {
        return recordsNum;
    }

    public float getMaxDelay() {
        return maxDelay;
    }

    public int getNumDelayed() {
        return numDelayed;
    }

    public int getNumCancelled() {
        return numCancelled;
    }

    public static float percentDelayed(DelayStatistics statistics) {
        return 100.0f * (float)statistics.numDelayed / (float)statistics.recordsNum;
    }

    public static float percentCancelled(DelayStatistics statistics) {
        return 100.0f * (float)statistics.numCancelled / (float)statistics.recordsNum;
    }

    public static float percentDelayedOrCancelled(DelayStatistics statistics) {
        return 100.0f * (float)(statistics.numCancelled + statistics.numDelayed) / (float)statistics.recordsNum;
    }

    public static DelayStatistics createNewDelayStatistics(float delay, boolean delayed, boolean cancelled) {
        DelayStatistics newStat = new DelayStatistics();
        newStat.recordsNum = 1;
        newStat.maxDelay     = delay;
        newStat.numDelayed   = delayed   ? 1 : 0;
        newStat.numCancelled = cancelled ? 1 : 0;
        return newStat;
    }


    public static DelayStatistics updateByNewRec(DelayStatistics statistics, float newDelay, boolean delayed, boolean cancelled) {
        statistics.recordsNum++;

        if (statistics.maxDelay < newDelay) {
            statistics.maxDelay = newDelay;
        }

        statistics.numDelayed   += delayed   ? 1 : 0;
        statistics.numCancelled += cancelled ? 1 : 0;

        return statistics;
    }

    public static DelayStatistics combineTwoStatistics(DelayStatistics a, DelayStatistics b) {
        a.recordsNum += b.recordsNum;
        a.numDelayed += b.numDelayed;
        a.numCancelled += b.numCancelled;

        return a;
    }

//    public static String resultStatistics(DelayStatistics statistics) {
//        return " max arr_delay: "
//                + Float.toString(statistics.maxDelay)
//                + "; cancelled and delayed: "
//                + Float.toString(percentDelayedOrCancelled(statistics)) + "%";
//    }
}