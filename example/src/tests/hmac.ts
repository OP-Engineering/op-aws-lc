import { generateHmacKey, HmacAlgorithm } from 'op-aws-lc';
import { describe, expect, it } from '../op-test';

describe('hmac', () => {
  it('hmac key can be generated', () => {
    const key = generateHmacKey(HmacAlgorithm.SHA256);
    expect(!!key).toBe(true);
  });

  it('hmac key can sign', () => {
    const key = generateHmacKey(HmacAlgorithm.SHA256);
    const signature = key.sign('hello');
    expect(!!signature).toBe(true);
  });

  it('hmac can sign and verify', () => {
    const key = generateHmacKey(HmacAlgorithm.SHA256);
    const signature = key.sign('hello');
    const result = key.verify('hello', signature);
    expect(result).toBe(true);
  });

  // it("hmac can't verify invalid signature", () => {
  //   const key = generateHmacKey(HmacAlgorithm.SHA256);
  //   const result = key.verify('hello', 'invalid');
  //   expect(result).toBe(false);
  // });

  it("hmac can't verify invalid data", () => {
    const key = generateHmacKey(HmacAlgorithm.SHA256);
    const signature = key.sign('hello');
    const result = key.verify('invalid', signature);
    expect(result).toBe(false);
  });
});
