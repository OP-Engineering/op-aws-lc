package com.opawslc;

import android.util.Log;

public class JNIOnLoad {
  private static final String TAG = "OPAwsLC";
  private static boolean isInitialized = false;

  public synchronized static void initializeNative() {
    if (isInitialized) return;
    try {
      System.loadLibrary("aws_lc_fips_0_13_1_crypto");
      System.loadLibrary("aws_lc_fips_0_13_1_rust_wrapper");
      System.loadLibrary("opawslc");
      isInitialized = true;
    } catch (Throwable e) {
      Log.e(TAG, "Failed to load libaws deps.", e);
      throw e;
    }
  }
}
