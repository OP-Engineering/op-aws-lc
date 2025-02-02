import OpAwsLc from './NativeOpAwsLc';

declare global {
  var __OPAwsLcProxy: any;
}

type HmacKey = {};

type Proxy = {
  generateHmacKey: () => HmacKey;
};

const errorMsg = OpAwsLc.install();

if (errorMsg != undefined) {
  console.error(`OpAwsLc could not be installed: ${errorMsg}`);
}

const proxy = global.__OPAwsLcProxy as Proxy;

if (proxy == null) {
  console.error(
    'OpAwsLc could not install JSI functions. Please check the native module implementation.'
  );
}

export function generateHmacKey() {
  return proxy.generateHmacKey();
}
