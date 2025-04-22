use rand::Rng;
use std::cmp::Ordering;
use std::io;

fn main() {
    println!("Guess the number!");
    let secret_number = rand::rng().random_range(1..=100);
    println!("The secret number is: {secret_number}");
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
        let guess: u32 = guess
            .trim()
            .parse()
            .expect("Please type a number!");
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
