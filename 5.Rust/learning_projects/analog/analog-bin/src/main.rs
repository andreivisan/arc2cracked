use std::{
    io::BufRead,
    result::Result
};
use thiserror::Error;

pub fn tally_status<R: BufRead>(reader: R) -> Result<Vec<(u16, u64)>, LogError> {

}
