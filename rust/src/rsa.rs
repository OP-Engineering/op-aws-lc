use std::ffi::{c_char, c_void};

use aws_lc_rs::{
    cipher,
    encoding::{AsDer, Pkcs8V1Der, PublicKeyX509Der},
    rsa::{
        self, KeySize, OaepPrivateDecryptingKey, OaepPublicEncryptingKey, PrivateDecryptingKey,
        PublicEncryptingKey, OAEP_SHA256_MGF1SHA256,
    },
};

use crate::{set_error, write_to_c_buffer};

#[repr(C)]
pub enum RsaKeySize {
    Rsa2048,
    Rsa3072,
    Rsa4096,
    Rsa8192,
}

#[repr(C)]
pub enum RsaOaepEncryptionAlgorithm {
    OaepSha1Mgf1Sha1,
    OaepSha256Mgf1Sha256,
    OaepSha384Mgf1Sha384,
    OaepSha512Mgf1Sha512,
}

impl From<RsaOaepEncryptionAlgorithm> for &rsa::OaepAlgorithm {
    fn from(algo: RsaOaepEncryptionAlgorithm) -> Self {
        match algo {
            RsaOaepEncryptionAlgorithm::OaepSha1Mgf1Sha1 => &rsa::OAEP_SHA1_MGF1SHA1,
            RsaOaepEncryptionAlgorithm::OaepSha256Mgf1Sha256 => &OAEP_SHA256_MGF1SHA256,
            RsaOaepEncryptionAlgorithm::OaepSha384Mgf1Sha384 => &rsa::OAEP_SHA384_MGF1SHA384,
            RsaOaepEncryptionAlgorithm::OaepSha512Mgf1Sha512 => &rsa::OAEP_SHA512_MGF1SHA512,
        }
    }
}

impl From<RsaKeySize> for KeySize {
    fn from(size: RsaKeySize) -> Self {
        match size {
            RsaKeySize::Rsa2048 => KeySize::Rsa2048,
            RsaKeySize::Rsa3072 => KeySize::Rsa3072,
            RsaKeySize::Rsa4096 => KeySize::Rsa4096,
            RsaKeySize::Rsa8192 => KeySize::Rsa8192,
        }
    }
}

#[repr(C)]
#[derive(Debug, thiserror::Error)]
pub enum RsaError {
    #[error("Ok")]
    Ok,
    #[error("Invalid key size")]
    InvalidKeySize,
    #[error("Invalid input")]
    InvalidInput,
    #[error("Invalid output")]
    InvalidOutput,
    #[error("Internal error")]
    CryptoError,
    #[error("Invalid key")]
    InvalidKey,
}

#[no_mangle]
pub unsafe extern "C" fn generate_rsa_private_key(
    key_size: RsaKeySize,
    key_handle: *mut *mut c_void,
    err: *mut *const c_char,
) -> RsaError {
    let key_size = key_size.into();
    match PrivateDecryptingKey::generate(key_size) {
        Ok(key) => {
            *key_handle = Box::into_raw(Box::new(key)) as *mut c_void;
            RsaError::Ok
        }
        Err(_) => RsaError::CryptoError,
    }
}

#[no_mangle]
pub unsafe extern "C" fn rsa_private_key_to_DER_PKCS8_bytes(
    private_key_handle: *mut c_void,
    output: *mut u8,
    output_len: *mut usize,
    err: *mut *const c_char,
) -> RsaError {
    if private_key_handle.is_null() {
        return RsaError::InvalidKey;
    }

    let private_key =
        unsafe { Box::from_raw(private_key_handle as *mut rsa::PrivateDecryptingKey) };
    let private_key_pkcs8 = private_key.as_der().unwrap();
    let private_key_bytes = private_key_pkcs8.as_ref();
    // Memory limit of the upper allocated buffer
    if private_key_bytes.len() > 1024 * 1024 {
        return RsaError::InvalidOutput;
    }
    unsafe {
        write_to_c_buffer(private_key_bytes, output, output_len);
    }
    std::mem::forget(private_key);

    RsaError::Ok
}

#[no_mangle]
pub unsafe extern "C" fn rsa_private_key_from_DER_PKCS8_bytes(
    private_key: *const u8,
    private_key_len: usize,
    private_key_handle: *mut *mut c_void,
    err: *mut *const c_char,
) -> RsaError {
    if private_key.is_null() || private_key_len == 0 {
        return RsaError::InvalidInput;
    }

    let private_key = std::slice::from_raw_parts(private_key, private_key_len);
    match PrivateDecryptingKey::from_pkcs8(private_key) {
        Ok(key) => {
            *private_key_handle = Box::into_raw(Box::new(key)) as *mut c_void;
            RsaError::Ok
        }
        Err(_) => RsaError::CryptoError,
    }
}

#[no_mangle]
pub unsafe extern "C" fn rsa_public_key_to_DER_X509_bytes(
    public_key_handle: *mut c_void,
    output: *mut u8,
    output_len: *mut usize,
    err: *mut *const c_char,
) -> RsaError {
    if public_key_handle.is_null() {
        return RsaError::InvalidKey;
    }

    let public_key = unsafe { Box::from_raw(public_key_handle as *mut rsa::PublicEncryptingKey) };
    let public_key_der = public_key.as_der().unwrap();
    let public_key_bytes = public_key_der.as_ref();

    if public_key_bytes.len() > 1024 * 1024 {
        return RsaError::InvalidOutput;
    }

    unsafe {
        write_to_c_buffer(public_key_bytes, output, output_len);
    }
    std::mem::forget(public_key);

    RsaError::Ok
}

#[no_mangle]
pub unsafe extern "C" fn rsa_public_key_from_DER_X509_bytes(
    public_key: *const u8,
    public_key_len: usize,
    public_key_handle: *mut *mut c_void,
    err: *mut *const c_char,
) -> RsaError {
    if public_key.is_null() || public_key_len == 0 {
        return RsaError::InvalidInput;
    }

    let public_key = std::slice::from_raw_parts(public_key, public_key_len);
    match PublicEncryptingKey::from_der(public_key) {
        Ok(key) => {
            *public_key_handle = Box::into_raw(Box::new(key)) as *mut c_void;
            RsaError::Ok
        }
        Err(_) => RsaError::CryptoError,
    }
}

#[no_mangle]
pub unsafe extern "C" fn generate_rsa_public_key(
    private_key_handle: *mut c_void,
    public_key_handle: *mut *mut c_void,
    err: *mut *const c_char,
) -> RsaError {
    if private_key_handle.is_null() {
        set_error(err, RsaError::InvalidKey.to_string().as_str());
        return RsaError::InvalidInput;
    }

    let key = unsafe { Box::from_raw(private_key_handle as *mut rsa::PrivateDecryptingKey) };
    let public_key = key.public_key();
    *public_key_handle = Box::into_raw(Box::new(public_key)) as *mut c_void;
    std::mem::forget(key);
    RsaError::Ok
}

#[no_mangle]
pub unsafe extern "C" fn generate_rsa_oaep_key(
    public_key_handle: *mut c_void,
    public_oaep_key_handle: *mut *mut c_void,
    output_len: *mut usize,
    err: *mut *const c_char,
) -> RsaError {
    if public_key_handle.is_null() {
        return RsaError::InvalidKey;
    }

    let public_key = unsafe { Box::from_raw(public_key_handle as *mut rsa::PublicEncryptingKey) };
    // TODO remove unwrap
    let oaep_public_key = OaepPublicEncryptingKey::new(*public_key).unwrap();
    let ciphertext_size = oaep_public_key.ciphertext_size();
    unsafe {
        *output_len = ciphertext_size;
        *public_oaep_key_handle = Box::into_raw(Box::new(oaep_public_key)) as *mut c_void;
    }

    RsaError::Ok
}

#[no_mangle]
pub unsafe extern "C" fn encrypt_with_rsa_oaep_key(
    algorithm: RsaOaepEncryptionAlgorithm,
    oaep_public_key_handle: *mut c_void,
    plaintext: *const u8,
    plaintext_len: usize,
    output: *mut u8,
    output_len: *mut usize,
    err: *mut *const c_char,
) -> RsaError {
    if oaep_public_key_handle.is_null() {
        return RsaError::InvalidKey;
    }

    if plaintext.is_null() || plaintext_len == 0 {
        return RsaError::InvalidInput;
    }

    let plaintext = std::slice::from_raw_parts(plaintext, plaintext_len);

    let oaep_public_key =
        unsafe { Box::from_raw(oaep_public_key_handle as *mut rsa::OaepPublicEncryptingKey) };

    let mut ciphertext = vec![0u8; oaep_public_key.ciphertext_size()];

    let ciphertext = oaep_public_key
        .encrypt(algorithm.into(), plaintext, &mut ciphertext, None)
        .unwrap();

    unsafe {
        write_to_c_buffer(ciphertext, output, output_len);
    }

    RsaError::Ok
}

#[no_mangle]
pub unsafe extern "C" fn rsa_oaep_decrypt(
    algorithm: RsaOaepEncryptionAlgorithm,
    private_key_handle: *mut c_void,
    ciphertext: *const u8,
    ciphertext_len: usize,
    output: *mut u8,
    output_len: *mut usize,
    err: *mut *const c_char,
) -> RsaError {
    log::error!("Starting decryption!");
    if private_key_handle.is_null() {
        return RsaError::InvalidKey;
    }

    if ciphertext.is_null() || ciphertext_len == 0 {
        return RsaError::InvalidInput;
    }

    let ciphertext = std::slice::from_raw_parts(ciphertext, ciphertext_len);

    let private_key = unsafe { &*(private_key_handle as *mut rsa::PrivateDecryptingKey) };

    // TODO unwrap
    let oaep_private_key = OaepPrivateDecryptingKey::new(private_key.clone()).unwrap();

    let mut plaintext = vec![0u8; oaep_private_key.min_output_size()];

    let plaintext = oaep_private_key
        .decrypt(algorithm.into(), ciphertext, &mut plaintext, None)
        .unwrap();

    log::error!("Plain text success! QUACK");

    unsafe {
        write_to_c_buffer(plaintext, output, output_len);
    }

    RsaError::Ok
}
