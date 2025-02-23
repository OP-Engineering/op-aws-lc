use aws_lc_rs::{hmac, rand};
use std::ffi::c_void;

#[no_mangle]
pub extern "C" fn generate_hmac_sha256_key(key_ptr: *mut *mut c_void) -> i32 {
    if key_ptr.is_null() {
        return 1;
    }

    let rng = rand::SystemRandom::new();
    let key = hmac::Key::generate(hmac::HMAC_SHA256, &rng);
    match key {
        Ok(k) => {
            let boxed_key = Box::new(k);
            unsafe {
                *key_ptr = Box::into_raw(boxed_key) as *mut c_void;
            }
            return 0;
        }
        Err(e) => {
            println!("Error: {:?}", e);
            return 1;
        }
    }
    // let key_str = CString::new(key.as_bytes()).unwrap();
}

#[no_mangle]
pub extern "C" fn retake_key(raw_ptr: *mut c_void) {
    unsafe {
        let boxed_key = Box::from_raw(raw_ptr as *mut hmac::Key);

        // Call std::mem::forget to prevent automatic deallocation when boxed_key goes out of scope
        // std::mem::forget(boxed_key); // This prevents the Box from being dropped and deallocated

        // boxed_key // You can still use `boxed_key` here if needed before forgetting
    }
}
