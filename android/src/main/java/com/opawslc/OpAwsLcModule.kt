package com.opawslc

import androidx.annotation.Keep
import com.facebook.proguard.annotations.DoNotStrip
import com.facebook.react.bridge.ReactApplicationContext
import com.facebook.react.bridge.ReactMethod
import com.facebook.react.common.annotations.FrameworkAPI
import com.facebook.react.module.annotations.ReactModule
import com.facebook.react.turbomodule.core.CallInvokerHolderImpl
import android.util.Log;

@DoNotStrip
@Keep
@OptIn(FrameworkAPI::class)
@ReactModule(name = OpAwsLcModule.NAME)
class OpAwsLcModule(reactContext: ReactApplicationContext) :
  NativeOpAwsLcSpec(reactContext) {

  override fun getName(): String {
    return NAME
  }

  @ReactMethod(isBlockingSynchronousMethod = true)
  override fun install(): Boolean {
    try {
      // 1. Get jsi::Runtime pointer
      val jsContext = reactApplicationContext.javaScriptContextHolder
        ?: return false

      // 2. Get CallInvokerHolder
      val callInvokerHolder = reactApplicationContext.jsCallInvokerHolder as? CallInvokerHolderImpl
        ?: return false

      install(jsContext.get(), callInvokerHolder)

      return true
    } catch (e: Throwable) {
      // ?. Something went wrong! Maybe a JNI error?
      return false
    }
  }

  private external fun install(
    jsRuntimePointer: Long,
    callInvokerHolder: CallInvokerHolderImpl,
  )

  companion object {
    const val NAME = "OpAwsLc"

    @JvmStatic
    var applicationContext: ReactApplicationContext? = null

    init {
      System.loadLibrary("aws_lc_fips_0_13_1_crypto")
      System.loadLibrary("aws_lc_fips_0_13_1_rust_wrapper")
      System.loadLibrary("aws_lc")
      System.loadLibrary("op-aws-lc");
      Log.i("OpAwsLc", "\uD83D\uDFE2 OpAwsLc Native Lib Loaded");
    }
  }
}
