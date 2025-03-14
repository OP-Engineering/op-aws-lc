import { rsa } from '@op-engineering/op-aws-lc';
import { describe, expect, it } from '../op-test';

describe('rsa', () => {
  it('full', () => {
    const privateKey = rsa.Key.generatePrivateKey(1);
    const publicKey = rsa.Key.generatePublicKey(privateKey);
    const outputBuffer = rsa.oaepEncrypt(1, publicKey, 'Hello world');
    expect(!!outputBuffer).toBe(true);
    expect(outputBuffer instanceof ArrayBuffer).toBe(true);
    // expect(outputBuffer.byteLength).toBeGreaterThan(0);
    // If you want to check for a specific length, you can use:
    // expect(outputBuffer.byteLength).toBe(expectedLength);
    // console.log(typeof outputBuffer);
  });
});
