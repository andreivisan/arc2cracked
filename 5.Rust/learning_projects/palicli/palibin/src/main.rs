use std::io;


use palilib::is_palindrome;


fn main() {
    for line in io::stdin().lines() {
        match line {
            Ok(input) => {
               println!("{}", if is_palindrome(input.trim_end()) { "YES" } else { "NO" }); 
            },
            Err(e) => eprintln!("Error reading line: {}", e),
        }
    }
}
