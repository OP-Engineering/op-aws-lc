import OpAwsLc from './NativeOpAwsLc';

declare global {
  var __OPAwsLcProxy: any;
}

type HmacKey = {
  sign: (data: string) => string;
  verify: (data: string, signature: string) => boolean;
};

type Proxy = {
  generateHmacKey: (algorithm: HmacAlgorithm) => HmacKey;
};

const errorMsg = OpAwsLc.install();

// eslint-disable-next-line eqeqeq
if (errorMsg != undefined) {
  console.error(`OpAwsLc could not be installed: ${errorMsg}`);
}

const proxy = global.__OPAwsLcProxy as Proxy;

if (proxy == null) {
  console.error(
    'OpAwsLc could not install JSI functions. Please check the native module implementation.'
  );
}

export enum HmacAlgorithm {
  SHA256 = 0,
  SHA384 = 1,
  SHA512 = 2,
}

export function generateHmacKey(algorithm: HmacAlgorithm): HmacKey {
  return proxy.generateHmacKey(algorithm);
}
