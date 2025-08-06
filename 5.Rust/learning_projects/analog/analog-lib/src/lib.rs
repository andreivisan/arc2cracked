use std::{
    io::BufRead,
    result::Result
};
use thiserror::Error;

#[derive(Debug, Error)]
pub enum LogError {
    #[error("I/O while reading log: {0}")]
    Io(#[from] std::io::Error),

    #[error("failed to parse line {line_no}")]
    ParseLine {
        line_no: usize,
        #[source]
        source: ParseLineErr,
    },
    
    #[error("status code overflowed u16 couter")]
    Overflow,
}

pub struct LogParser<R> {
    reader: R,
    buff: String,
}

impl<R: BufRead> LogParser<R> {
    pub fn new(reader: R) -> Self {
        Self {
            reader,
            buff: String::new()
        }
    }
}

impl<R:BufRead, ParseLineErr> Iterator for LogParser<R> {
   type Item = Result<u16, ParseLineErr>; 

   fn next(&mut self) -> Option<Self::Item> {
        
   }
}

// pub fn tally_status<R: BufRead>(reader: R) -> Result<Vec<(u16, u64)>, LogError> {
//
// }

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn it_works() {
        let result = add(2, 2);
        assert_eq!(result, 4);
    }
}
