import OpAwsLc from "./NativeOpAwsLc";

declare global {
  var __OPAwsLcProxy: any;
}

type HmacKey = ArrayBuffer;
type RsaPrivateKey = Object;
type RsaPublicKey = Object;

type Proxy = {
  hmac: {
    signAsync: (key: HmacKey, data: string) => Promise<HmacKey>;
    sign: (key: HmacKey, data: string) => HmacKey;
    verifyAsync: (
      key: HmacKey,
      data: string,
      signature: HmacKey
    ) => Promise<boolean>;
    verify: (key: HmacKey, data: string, signature: HmacKey) => boolean;
    Key: {
      generate: (algorithm: HmacAlgorithm) => HmacKey;
    };
  };
  generateHmacKey: (algorithm: HmacAlgorithm) => HmacKey;
  rsa: {
    Key: {
      generatePrivateKey: (keySize: RsaKeySize) => RsaPrivateKey;
      generatePublicKey: (privateKey: RsaPrivateKey) => RsaPublicKey;
      privateKeyToDERPKCS8Bytes: (privateKey: RsaPrivateKey) => ArrayBuffer;
      privateKeyFromDERPKCS8Bytes: (bytes: ArrayBuffer) => RsaPrivateKey;
      publicKeyToDERX509Bytes: (publicKey: RsaPublicKey) => ArrayBuffer;
      publicKeyFromDERX509Bytes: (bytes: ArrayBuffer) => RsaPublicKey;
    };
    /**
     *
     * @param label is an optional label to associate with the message. If provided, it must be used when decrypting the message.
     * @returns
     */
    oaepEncrypt: (params: {
      algorithm: RsaOaepEncryptionAlgorithm;
      publicKey: RsaPublicKey;
      plaintext: string;
      label?: string;
    }) => ArrayBuffer;
    /**
     *
     * @param label is an optional label to associate with the message. If provided, it must be the same as the label used when encrypting the message.
     * @returns
     */
    oaepDecrypt: (params: {
      algorithm: RsaOaepEncryptionAlgorithm;
      privateKey: RsaPrivateKey;
      ciphertext: ArrayBuffer;
      label?: string;
    }) => string;
  };
};

const installed = OpAwsLc.install();

// eslint-disable-next-line
if (!installed) {
  console.error(`OpAwsLc could not be installed! Check native logs!`);
}

const proxy = global.__OPAwsLcProxy as Proxy;

if (proxy == null) {
  console.error(
    "OpAwsLc could not install JSI functions. Please check the native module implementation."
  );
}

export enum HmacAlgorithm {
  SHA256 = 0,
  SHA384 = 1,
  SHA512 = 2,
}

export enum RsaKeySize {
  Rsa2048 = 0,
  Rsa3072 = 1,
  Rsa4096 = 2,
  Rsa8192 = 3,
}

export enum RsaOaepEncryptionAlgorithm {
  Sha1mgf1Sha1 = 0,
  Sha256mgf1Sha256 = 1,
  Sha384mgf1Sha384 = 2,
  Sha512mgf1Sha512 = 3,
}

export const hmac = proxy.hmac;
export const rsa = proxy.rsa;
