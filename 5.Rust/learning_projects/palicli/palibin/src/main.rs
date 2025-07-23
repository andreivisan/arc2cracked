use std::io;


use palilib::is_palindrome;


fn main() {
    for line in io::stdin().lines() {
        match line {
            Ok(input) => {
                let ispali = is_palindrome(&input);
                if ispali {
                    println!("YES");
                } else {
                    println!("NO");
                }
            },
            Err(e) => eprintln!("Error reading line: {}", e),
        }
    }
}
