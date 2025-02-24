import OpAwsLc from './NativeOpAwsLc';

declare global {
  var __OPAwsLcProxy: any;
}

type HmacKey = ArrayBuffer;

type Proxy = {
  hmac: {
    signAsync: (key: HmacKey, data: string) => Promise<HmacKey>;
    sign: (key: HmacKey, data: string) => HmacKey;
    verifyAsync: (
      key: HmacKey,
      data: string,
      signature: HmacKey
    ) => Promise<boolean>;
    verify: (key: HmacKey, data: string, signature: HmacKey) => boolean;
    Key: {
      generate: (algorithm: HmacAlgorithm) => HmacKey;
    };
  };
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

export const hmac = proxy.hmac;
