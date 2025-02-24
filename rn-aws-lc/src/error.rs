use std::ffi::c_char;
use std::ptr;
use std::sync::Mutex;

lazy_static::lazy_static! {
    static ref GLOBAL_ERROR: Mutex<Option<String>> = Mutex::new(None);
}

/// # Safety
///
/// The calling context should have passed a valid pointer to set the error to
pub unsafe fn set_error(err: *mut *const c_char, error_message: &str) {
    let mut global_error = GLOBAL_ERROR.lock().unwrap();
    *global_error = Some(error_message.to_string());
    *err = error_message.as_ptr() as *const c_char;
}

#[no_mangle]
pub extern "C" fn get_error() -> *const i8 {
    let global_error = GLOBAL_ERROR.lock().unwrap();
    match &*global_error {
        Some(err) => err.as_ptr() as *const i8,
        None => ptr::null(),
    }
}
