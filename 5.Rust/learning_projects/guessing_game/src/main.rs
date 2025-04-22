use rand::Rng;
use std::cmp::Ordering;
use std::io;

fn main() {
    println!("Guess the number!");
    let secret_number = rand::rng().random_range(1..=100);
    loop {
        println!("Please input your guess.");
        //new is an associated func-tion of the String type
        //An associated function is a function that’s implemented
        //on a type, in this case String.
        let mut guess = String::new();
        io::stdin()
            //reference to guess so that multiple parts
            //of the program can access guess without
            //copying it
            .read_line(&mut guess)
            .expect("Failed to read line");
        // when we use an enum (what parse return an enum Result
        // with OK and Err, we use match to tell Rust what to do
        // if match OK return num if match Err continue. These
        // are called arms in Rust. 
        let guess: u32 = match guess.trim().parse() {
            Ok(num) => num,
            Err(_) => continue,
        };
        println!("You guessed: {guess}");
        match guess.cmp(&secret_number) {
            Ordering::Less => println!("Too small!"),
            Ordering::Greater => println!("Too big!"),
            Ordering::Equal => {
                println!("You win!");
                break;
            }
        }
    }
}
