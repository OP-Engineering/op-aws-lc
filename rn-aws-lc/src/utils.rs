use std::ptr;

/// # Safety
/// All pointers should be valid
pub unsafe fn write_to_c_buffer(data: &[u8], out_data: *mut u8, out_len: *mut usize) {
    ptr::copy_nonoverlapping(data.as_ptr(), out_data, data.len());
    *out_len = data.len();
}
