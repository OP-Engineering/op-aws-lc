import OpAwsLc from './NativeOpAwsLc';

declare global {
  var __OPAwsLcProxy: object | undefined;
}

type Proxy = {
  hmac: () => string;
};

OpAwsLc.install();

if (__OPAwsLcProxy !== undefined) {
  console.error(
    'OpAwsLc could not install JSI functions. Please check the native module implementation.'
  );
}

const proxy = global.__OPAwsLcProxy! as Proxy;

export function hmac() {
  return proxy.hmac();
}
