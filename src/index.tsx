import OpAwsLc from './NativeOpAwsLc';

declare global {
  var __OPAwsLcProxy: object | undefined;
}

OpAwsLc.install();

if (__OPAwsLcProxy !== undefined) {
  console.error(
    'OpAwsLc could not install JSI functions. Please check the native module implementation.'
  );
}
