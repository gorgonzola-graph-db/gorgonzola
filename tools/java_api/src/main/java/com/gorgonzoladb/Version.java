package com.gorgonzoladb;

/**
 * Version is a class to get the version of the Gorgonzola.
 */
public class Version {

    /**
     * Get the version of the Gorgonzola.
     *
     * @return The version of the Gorgonzola.
     */
    public static String getVersion() {
        return Native.gorgonzolaGetVersion();
    }

    /**
     * Get the storage version of the Gorgonzola.
     *
     * @return The storage version of the Gorgonzola.
     */
    public static long getStorageVersion() {
        return Native.gorgonzolaGetStorageVersion();
    }
}
