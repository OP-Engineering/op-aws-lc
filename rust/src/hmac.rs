use crate::{set_error, utils::write_to_c_buffer};
use aws_lc_rs::{hmac, rand};
use std::{
    ffi::{c_char, c_void},
    slice,
};

#[repr(C)]
pub enum CHmacAlgorithm {
    SHA256,
    SHA384,
    SHA512,
}

impl From<CHmacAlgorithm> for hmac::Algorithm {
    fn from(algo: CHmacAlgorithm) -> Self {
        match algo {
            CHmacAlgorithm::SHA256 => hmac::HMAC_SHA256,
            CHmacAlgorithm::SHA384 => hmac::HMAC_SHA384,
            CHmacAlgorithm::SHA512 => hmac::HMAC_SHA512,
        }
    }
}

#[repr(C)]
#[derive(Debug, thiserror::Error)]
pub enum CHmacError {
    #[error("Ok")]
    Ok,
    #[error("Invalid key")]
    InvalidKey,
    #[error("Invalid algorithm")]
    InvalidAlgorithm,
    #[error("Invalid input")]
    InvalidInput,
    #[error("Invalid output")]
    InvalidOutput,
    #[error("Internal error")]
    CryptoError,
}

impl From<aws_lc_rs::error::Unspecified> for CHmacError {
    fn from(_: aws_lc_rs::error::Unspecified) -> Self {
        CHmacError::CryptoError
    }
}

/// # Safety
/// All pointers should be valid
#[no_mangle]
pub unsafe extern "C" fn generate_hmac_key(
    algorithm: CHmacAlgorithm,
    key_handle: *mut *mut c_void,
    err: *mut *mut c_char,
) -> CHmacError {
    let rng = rand::SystemRandom::new();
    match hmac::Key::generate(algorithm.into(), &rng) {
        Ok(key) => {
            *key_handle = Box::into_raw(Box::new(key)) as *mut c_void;

            CHmacError::Ok
        }
        Err(e) => {
            let e = format!("{:?}", e);
            let e = std::ffi::CString::new(e).unwrap();
            unsafe {
                *err = e.into_raw();
            }
            CHmacError::CryptoError
        }
    }
}

/// # Safety
/// All pointers should be valid
#[no_mangle]
pub unsafe extern "C" fn hmac_sign(
    key_handle: *mut c_void,
    data: *const u8,
    data_len: usize,
    output: *mut u8,
    output_len: *mut usize,
    err: *mut *const c_char,
) -> CHmacError {
    if key_handle.is_null() {
        set_error(err, CHmacError::InvalidKey.to_string().as_str());

        return CHmacError::InvalidKey;
    }

    if data.is_null() || data_len == 0 {
        unsafe {
            set_error(err, CHmacError::InvalidInput.to_string().as_str());
        }
        return CHmacError::InvalidInput;
    }

    if output.is_null() || output_len.is_null() {
        unsafe {
            set_error(err, CHmacError::InvalidOutput.to_string().as_str());
        }
        return CHmacError::InvalidOutput;
    }

    let key = unsafe { Box::from_raw(key_handle as *mut hmac::Key) };
    let msg = unsafe { slice::from_raw_parts(data, data_len) };
    let tag: hmac::Tag = hmac::sign(&key, msg);

    unsafe {
        write_to_c_buffer(tag.as_ref(), output, output_len);
    }

    // Prevent deallocation of the key
    std::mem::forget(key);

    CHmacError::Ok
}

/// # Safety
/// All pointers should be valid
#[no_mangle]
pub unsafe extern "C" fn hmac_verify(
    key_handle: *mut c_void,
    data: *const u8,
    data_len: usize,
    tag: *const u8,
    tag_len: usize,
) -> CHmacError {
    if key_handle.is_null() {
        return CHmacError::InvalidKey;
    }

    if data.is_null() || data_len == 0 {
        return CHmacError::InvalidInput;
    }

    if tag.is_null() || tag_len == 0 {
        return CHmacError::InvalidInput;
    }

    let key = Box::from_raw(key_handle as *mut hmac::Key);
    let msg = slice::from_raw_parts(data, data_len);
    let tag = slice::from_raw_parts(tag, tag_len);

    let result = hmac::verify(&key, msg, tag);

    if result.is_err() {
        return CHmacError::CryptoError;
    }

    // Prevent deallocation of the key
    std::mem::forget(key);

    CHmacError::Ok
}

#[no_mangle]
pub extern "C" fn hmac_destroy(key_handle: *mut c_void) {
    if key_handle.is_null() {
        return;
    }

    let _key = unsafe { Box::from_raw(key_handle as *mut hmac::Key) };
}
