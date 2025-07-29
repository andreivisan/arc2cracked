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

pub fn tally_status<R: BufRead>(reader: R) -> Result<Vec<(u16, u64)>, LogError> {

}
