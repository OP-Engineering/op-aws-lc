import {
  rsa,
  RsaKeySize,
  RsaOaepEncryptionAlgorithm,
} from '@op-engineering/op-aws-lc';
import { describe, expect, it } from '../op-test';

describe('rsa', () => {
  it('keySize2048AndSha256', () => {
    const privateKey = rsa.Key.generatePrivateKey(RsaKeySize.Rsa2048);
    const publicKey = rsa.Key.generatePublicKey(privateKey);
    const ciphertext = rsa.oaepEncrypt({
      algorithm: RsaOaepEncryptionAlgorithm.Sha256mgf1Sha256,
      publicKey,
      plaintext: 'Hello world',
    });
    expect(!!ciphertext).toBe(true);
    expect(ciphertext instanceof ArrayBuffer).toBe(true);
    const decrypted = rsa.oaepDecrypt({
      algorithm: RsaOaepEncryptionAlgorithm.Sha256mgf1Sha256,
      privateKey,
      ciphertext,
    });
    expect(decrypted).toBe('Hello world');
  });

  // it('keySize3072AndSha256', () => {
  //   const privateKey = rsa.Key.generatePrivateKey(RsaKeySize.Rsa3072);
  //   const publicKey = rsa.Key.generatePublicKey(privateKey);
  //   const ciphertext = rsa.oaepEncrypt({
  //     algorithm: RsaOaepEncryptionAlgorithm.Sha256mgf1Sha256,
  //     publicKey,
  //     plaintext: 'Hello world',
  //   });
  //   expect(!!ciphertext).toBe(true);
  //   expect(ciphertext instanceof ArrayBuffer).toBe(true);
  //   const decrypted = rsa.oaepDecrypt({
  //     algorithm: RsaOaepEncryptionAlgorithm.Sha256mgf1Sha256,
  //     privateKey,
  //     ciphertext,
  //   });
  //   expect(decrypted).toBe('Hello world');
  // });

  // it('keySize8192AndSha384', () => {
  //   const privateKey = rsa.Key.generatePrivateKey(RsaKeySize.Rsa8192);
  //   const publicKey = rsa.Key.generatePublicKey(privateKey);
  //   const ciphertext = rsa.oaepEncrypt({
  //     algorithm: RsaOaepEncryptionAlgorithm.Sha384mgf1Sha384,
  //     publicKey,
  //     plaintext: 'Hello world',
  //   });
  //   expect(!!ciphertext).toBe(true);
  //   expect(ciphertext instanceof ArrayBuffer).toBe(true);
  //   const decrypted = rsa.oaepDecrypt({
  //     algorithm: RsaOaepEncryptionAlgorithm.Sha384mgf1Sha384,
  //     privateKey,
  //     ciphertext,
  //   });
  //   expect(decrypted).toBe('Hello world');
  // });

  // it('keySize8192AndSha512', () => {
  //   const privateKey = rsa.Key.generatePrivateKey(RsaKeySize.Rsa8192);
  //   const publicKey = rsa.Key.generatePublicKey(privateKey);
  //   const ciphertext = rsa.oaepEncrypt({
  //     algorithm: RsaOaepEncryptionAlgorithm.Sha512mgf1Sha512,
  //     publicKey,
  //     plaintext: 'Hello world',
  //   });
  //   expect(!!ciphertext).toBe(true);
  //   expect(ciphertext instanceof ArrayBuffer).toBe(true);
  //   const decrypted = rsa.oaepDecrypt({
  //     algorithm: RsaOaepEncryptionAlgorithm.Sha512mgf1Sha512,
  //     privateKey,
  //     ciphertext,
  //   });
  //   expect(decrypted).toBe('Hello world');
  // });

  it('Can export and import a private key to DER PCKS8', () => {
    const privateKey = rsa.Key.generatePrivateKey(RsaKeySize.Rsa2048);
    const privateKeyBytes = rsa.Key.privateKeyToDERPKCS8Bytes(privateKey);

    expect(privateKeyBytes instanceof ArrayBuffer).toBe(true);

    const privateKeyFromBytes =
      rsa.Key.privateKeyFromDERPKCS8Bytes(privateKeyBytes);
    // expect(privateKeyFromBytes).toEqual(privateKey);
    const publicKey = rsa.Key.generatePublicKey(privateKeyFromBytes);
    const ciphertext = rsa.oaepEncrypt({
      algorithm: RsaOaepEncryptionAlgorithm.Sha512mgf1Sha512,
      publicKey,
      plaintext: 'Hello world',
    });
    expect(!!ciphertext).toBe(true);
    expect(ciphertext instanceof ArrayBuffer).toBe(true);
    const decrypted = rsa.oaepDecrypt({
      algorithm: RsaOaepEncryptionAlgorithm.Sha512mgf1Sha512,
      privateKey,
      ciphertext,
    });
    expect(decrypted).toBe('Hello world');
  });

  it('Can export and import a public key to DER X509', () => {
    const privateKey = rsa.Key.generatePrivateKey(RsaKeySize.Rsa2048);
    const publicKey = rsa.Key.generatePublicKey(privateKey);
    const publicKeyBytes = rsa.Key.publicKeyToDERX509Bytes(publicKey);

    expect(publicKeyBytes instanceof ArrayBuffer).toBe(true);

    const publicKeyFromBytes =
      rsa.Key.publicKeyFromDERX509Bytes(publicKeyBytes);
    // expect(publicKeyFromBytes).toEqual(publicKey);
    const ciphertext = rsa.oaepEncrypt({
      algorithm: RsaOaepEncryptionAlgorithm.Sha512mgf1Sha512,
      publicKey: publicKeyFromBytes,
      plaintext: 'Hello world',
    });
    expect(!!ciphertext).toBe(true);
    expect(ciphertext instanceof ArrayBuffer).toBe(true);
    const decrypted = rsa.oaepDecrypt({
      algorithm: RsaOaepEncryptionAlgorithm.Sha512mgf1Sha512,
      privateKey,
      ciphertext,
    });
    expect(decrypted).toBe('Hello world');
  });
});
