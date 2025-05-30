// This is a quiz for the following sections:
// - Strings
// - Vecs
// - Move semantics
// - Modules
// - Enums
//
// Let's build a little machine in the form of a function. As input, we're going
// to give a list of strings and commands. These commands determine what action
// is going to be applied to the string. It can either be:
// - Uppercase the string
// - Trim the string
// - Append "bar" to the string a specified amount of times
//
// The exact form of this will be:
// - The input is going to be a Vector of 2-length tuples,
//   the first element is the string, the second one is the command.
// - The output element is going to be a vector of strings.

pub enum Command {
    Uppercase,
    Trim,
    Append(usize),
}

pub mod my_module {
    use super::Command;

    // TODO: Complete the function as described above.
    pub fn transformer(input: Vec<(String, Command)>) -> Vec<String> {
        // let mut result = Vec::new();
        // for (mut string, command) in input {
        //    match command {
        //        Command::Uppercase => {
        //            result.push(string.to_uppercase());
        //        }
        //        Command::Trim => {
        //            result.push(string.trim().to_string());
        //        }
        //        Command::Append(val) => {
        //            string.push_str(&"bar".repeat(val));
        //            result.push(string);
        //        }
        //    } 
        // }
        // result
        input.into_iter()                // consume the Vec, no cloning
         .map(|(mut s, cmd)| match cmd {
             Command::Uppercase => s.to_uppercase(),
             Command::Trim      => s.trim().to_string(),
             Command::Append(n) => {
                 s.reserve(3 * n);   // one grow step for "bar" * n
                 for _ in 0..n {
                     s.push_str("bar");
                 }
                 s
             }
         })
         .collect()
    }
}

fn main() {
    // You can optionally experiment here.
}

#[cfg(test)]
mod tests {
    // TODO: What do we need to import to have `transformer` in scope?
    use crate::my_module::transformer;
    use super::Command;

    #[test]
    fn it_works() {
        let input = vec![
            ("hello".to_string(), Command::Uppercase),
            (" all roads lead to rome! ".to_string(), Command::Trim),
            ("foo".to_string(), Command::Append(1)),
            ("bar".to_string(), Command::Append(5)),
        ];
        let output = transformer(input);

        assert_eq!(
            output,
            [
                "HELLO",
                "all roads lead to rome!",
                "foobar",
                "barbarbarbarbarbar",
            ]
        );
    }
}
