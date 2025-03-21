# op-aws-lc

AWS LC with FIPS for React Native. AWS-LC is Amazons LibCrypto. Besides being tailored for their specific use-cases, it is battle tested and constantly updated. This library uses the Rust version for ease of compiling but internally it runs the same code as the base AWS-LC version (it contains code from BoringSSL and handwritten assembly).

AWS-LC-RS follows closely the Ring API, aiming to be a drop-in replacement, this library also tries to mimic the API in a JS idiomatic way.

## Installation

```sh
npm install @op-engineering/op-aws-lc
npx pod-install
```

Or if using Expo

```sh
npm install @op-engineering/op-aws-lc
npx expo prebuild
```

Unfortunately, AWS-LC with FIPS does not compile for mac intel architectures, so if you add this library to your app it will not be able to be compiled for older Intel Macs.

## Motivation

The main motivation is not to bring yet another crypto library to the ecosystem, but rather a FIPS certified one for hardened security requirements.

FIPS (Federal Information Processing Standards) is a set of standards and guidelines for cryptographic modules, established by the US National Institute of Standards and Technology (NIST). FIPS 140-2/3 certification ensures that cryptographic modules meet rigorous security requirements for use in government and regulated industries.

Key points about FIPS:

- Required for US federal government systems
- Industry standard for healthcare and financial sectors
- Validates implementation of cryptographic algorithms
- Ensures proper key management and module security
- AWS-LC has obtained FIPS 140-2 Level 1 validation

This library binds the Rust version of AWS-LC with FIPS turned on.

[![Amazon LC Fips](https://img.youtube.com/vi/WKNGnCkZiQE/0.jpg)](https://www.youtube.com/watch?v=WKNGnCkZiQE)

## Usage

### Hmac

```ts
import { hmac, HmacAlgorithm } from '@op-engineering/op-aws-lc';

// key is an ArrayBuffer that contains the raw bytes of the tag
const key = hmac.Key.generate(HmacAlgorithm.SHA256);

const signature = hmac.sign(key, 'hello');

const verified = hmac.verify(key, 'hello', signature);

// There are also async versions when the data you are trying to verify is large
const signature = hmac.signAsync(key, 'hello');

const verified = hmac.verifyAsync(key, 'hello', signature);

if (!verified) {
  throw new Error('failed to pass hmac verfication');
}
```

### Rsa

Encryption/Decryption of plaintext with Rsa works. As well as importing/exporting of the private/public key into DER PKCS8/X509 formats.

````ts
import {rsa, RsaKeySize, RsaOaepEncryptionAlgorithm } from "@op-engineering/op-aws-lc";

const privateKey = rsa.Key.generatePrivateKey(RsaKeySize.Rsa2048);
const privateKeyBytes = rsa.Key.privateKeyToDERPKCS8Bytes(privateKey);

expect(privateKeyBytes instanceof ArrayBuffer).toBe(true);

const privateKeyFromBytes =
  rsa.Key.privateKeyFromDERPKCS8Bytes(privateKeyBytes);

const publicKey = rsa.Key.generatePublicKey(privateKeyFromBytes);
const publicKeyBytes = rsa.Key.publicKeyToDERX509Bytes(publicKey);

expect(publicKeyBytes instanceof ArrayBuffer).toBe(true);

const publicKeyFromBytes =
  rsa.Key.publicKeyFromDERX509Bytes(publicKeyBytes);

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
```


## Get in touch

The rest of the algorithms inside aws-lc still need to be bridged. Get in contact by [joining our Discord](https://discord.gg/W9XmqCQCKP) or write to ospfranco@gmail.com for further assistance.

You can see the [list of available funcitonality in the Rust crate here](https://docs.rs/aws-lc-rs/latest/aws_lc_rs/). Eventually op-aws-lc aims to cover all of these.

## License

MIT
````
