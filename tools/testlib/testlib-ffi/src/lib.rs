
use std::ffi::CStr;
use std::os::raw::c_char;

#[no_mangle]
pub extern fn simple_function(lhs:i32,rhs:i32) -> i32 {
    lhs + rhs
}

#[no_mangle]
pub extern fn cstring_function(ptr: *const c_char) -> usize {

    let cstr = unsafe { CStr::from_ptr(ptr) };

    match cstr.to_str() {
        Ok(s) => {
            s.chars().count()
        }
        Err(_) => 0
    }
}