import { hmac, HmacAlgorithm } from "@op-engineering/op-aws-lc";
import { describe, expect, it } from "@op-engineering/op-test";

describe("hmac", () => {
  it("hmac key can be generated", () => {
    const key = hmac.Key.generate(HmacAlgorithm.SHA256);
    expect(!!key).toBe(true);
  });

  it("hmac key can sign", () => {
    const key = hmac.Key.generate(HmacAlgorithm.SHA256);
    const signature = hmac.sign(key, "hello");
    expect(!!signature).toBe(true);
  });

  it("hmac can sign and verify", () => {
    const key = hmac.Key.generate(HmacAlgorithm.SHA256);
    const signature = hmac.sign(key, "hello");
    const result = hmac.verify(key, "hello", signature);
    expect(result).toBe(true);
  });

  it("hmac can't verify invalid signature", () => {
    const key = hmac.Key.generate(HmacAlgorithm.SHA256);
    const invalidSignature = new Uint8Array(32).map(() =>
      Math.floor(Math.random() * 256)
    );
    const result = hmac.verify(key, "hello", invalidSignature.buffer);
    expect(result).toBe(false);
  });

  it("hmac can't verify invalid data", () => {
    const key = hmac.Key.generate(HmacAlgorithm.SHA256);
    const signature = hmac.sign(key, "hello");
    const result = hmac.verify(key, "invalid", signature);
    expect(result).toBe(false);
  });

  it("hmac can sign async", async () => {
    const key = hmac.Key.generate(HmacAlgorithm.SHA256);
    const signature = await hmac.signAsync(key, "hello");
    expect(!!signature).toBe(true);
  });

  it("hmac can verify async", async () => {
    const key = hmac.Key.generate(HmacAlgorithm.SHA256);
    const signature = await hmac.signAsync(key, "hello");
    const result = await hmac.verifyAsync(key, "hello", signature);
    expect(result).toBe(true);
  });

  it("hmac verify async can reject invalid data", async () => {
    const key = hmac.Key.generate(HmacAlgorithm.SHA256);
    const signature = await hmac.signAsync(key, "hello");
    const result = await hmac.verifyAsync(key, "invalid", signature);
    expect(result).toBe(false);
  });
});
