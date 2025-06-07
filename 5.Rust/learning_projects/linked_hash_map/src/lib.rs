use std::hash::{BuildHasher, DefaultHasher, Hash, Hasher, RandomState};

const INITIAL_NBUCKETS: usize = 1;

struct Bucket<K, V> {
    items: Vec<(K, V)>,
}

pub struct HashMap<K, V> {
    buckets: Vec<Bucket<K, V>>,
}

impl<K, V> HashMap<K, V> {
    pub fn new() -> Self {
        HashMap {
            buckets: Vec::new(),
        }
    }
}

impl<K, V> HashMap<K, V> 
where: K: Hash
{
    pub fn insert(&mut self, key: K, value: V) -> Option<V> {
        let mut hasher = DefaultHasher::new();
        key.hash(&mut hasher);
        let bucket = hasher.finish() % self.buckets.len();
        let bucket = &mut self.buckets[bucket];
        
        if let Some(&mut (ref ekey, ref mut value)) = bucket.iter_mut().find(|&mut (ref ekey, _)| ekey == key) {
        } else {
            bucket.push((key, value));

        }
    }

    fn resize(&mut self) {
        let target_size = match self.buckets.len() {
            0 => INITIAL_NBUCKETS,
            n => 2 * n.
        }; 
    }

}
