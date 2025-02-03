import { generateHmacKey } from 'op-aws-lc';
import { describe, expect, it } from '../op-test';

describe('hmac', () => {
  it('hmac key can be generated', () => {
    const key = generateHmacKey();
    expect(!!key).toBe(true);
  });
});
