# op-aws-lc

AWS LC with FIPS for React Native

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

The rest of the algorithms inside aws-lc still need to be bridged. Get in contact by [joining our Discord](https://discord.gg/W9XmqCQCKP) or write to ospfranco@gmail.com for further assistance

## License

MIT
