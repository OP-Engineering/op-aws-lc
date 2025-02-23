# op-aws-lc

AWS LC with FIPS for React Native. AWS-LC is Amazons LibCrypto. Besides being tailored for their specific use-cases, it is battle tested and constantly updated. This library uses the Rust version for ease of compiling but internally it runs the same code as the base AWS-LC version (it contains code from BoringSSL and handwritten assembly).

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
import { generateHmacKey, HmacAlgorithm } from '@op-engineering/op-aws-lc';

const key = generateHmacKey(HmacAlgorithm.SHA256);
// Signature is an ArrayBuffer that contains the raw bytes of the tag
const signature = key.sign('hello');
if (key.verify('hello', signature) == false) {
  console.error('Signature verification failed!');
}
```

## Get in touch

The rest of the algorithms inside aws-lc still need to be bridged. Get in contact by [joining our Discord](https://discord.gg/W9XmqCQCKP) or write to ospfranco@gmail.com for further assistance.

You can see the [list of available funcitonality in the Rust crate here](https://docs.rs/aws-lc-rs/latest/aws_lc_rs/). Eventually op-aws-lc aims to cover all of these.

## License

MIT
