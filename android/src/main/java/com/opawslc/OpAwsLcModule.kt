package com.opawslc

import androidx.annotation.Keep
import com.facebook.proguard.annotations.DoNotStrip
import com.facebook.react.bridge.ReactApplicationContext
import com.facebook.react.common.annotations.FrameworkAPI
import com.facebook.react.module.annotations.ReactModule
import com.facebook.react.turbomodule.core.CallInvokerHolderImpl

@DoNotStrip
@Keep
@OptIn(FrameworkAPI::class)
@Suppress("KotlinJniMissingFunction")
class OpAwsLcModule(reactContext: ReactApplicationContext) :
  NativeOpAwsLcSpec(reactContext) {

  override fun getName(): String {
    return NAME
  }

  override fun install(): String? {
    try {
      // 1. Get jsi::Runtime pointer
      val jsContext = reactApplicationContext.javaScriptContextHolder
        ?: return "ReactApplicationContext.javaScriptContextHolder is null!"

      // 2. Get CallInvokerHolder
      val callInvokerHolder = reactApplicationContext.jsCallInvokerHolder as? CallInvokerHolderImpl
        ?: return "ReactApplicationContext.jsCallInvokerHolder is null!"

      // 3. Install Nitro
      install(jsContext.get(), callInvokerHolder)

      return null
    } catch (e: Throwable) {
      // ?. Something went wrong! Maybe a JNI error?
      return e.message
    }
  }

  private external fun install(jsRuntimePointer: Long, callInvokerHolder: CallInvokerHolderImpl)

  companion object {
    const val NAME = "OpAwsLc"

    init {
      JNIOnLoad.initializeNative()
    }
  }
}
