#[derive(PartialEq, Eq, Clone, Debug)]
pub struct ListNode {
    pub val: i32,
    pub next: Option<Box<ListNode>>
}

impl ListNode {
    #[inline]
    fn new(val: i32) -> Self {
        ListNode {
            next: None,
            val
        }
    }
}

pub fn merge_two_lists(list1: Option<Box<ListNode>>, list2: Option<Box<ListNode>>) -> Option<Box<ListNode>> {
    match (list1, list2) {
        (Some(l1), None) => Some(l1),
        (None, Some(l2)) => Some(l2),
        (Some(mut l1), Some(mut l2)) => {
            if l1.val <= l2.val {
                l1.next = merge_two_lists(l1.next, Some(l2));
                Some(l1)
            } else {
                l2.next = merge_two_lists(Some(l1), l2.next);
                Some(l2)
            }
        },
        (None, None) => None
    }         
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_merge_two_lists_empty() {
        // Test merging two empty lists
        let list1 = None;
        let list2 = None;
        assert_eq!(merge_two_lists(list1, list2), None);
    }

    #[test]
    fn test_merge_two_lists_one_empty() {
        // Test merging when one list is empty
        let list1 = Some(Box::new(ListNode::new(1)));
        let list2 = None;
        assert_eq!(merge_two_lists(list1.clone(), list2), list1);

        let list1 = None;
        let list2 = Some(Box::new(ListNode::new(2)));
        assert_eq!(merge_two_lists(list1, list2.clone()), list2);
    }

    #[test]
    fn test_merge_two_lists_sorted() {
        // Test merging two sorted lists with distinct values
        let mut list1 = Some(Box::new(ListNode::new(1)));
        list1.as_mut().unwrap().next = Some(Box::new(ListNode::new(3)));

        let mut list2 = Some(Box::new(ListNode::new(2)));
        list2.as_mut().unwrap().next = Some(Box::new(ListNode::new(4)));

        let merged = merge_two_lists(list1, list2);

        let mut expected = Some(Box::new(ListNode::new(1)));
        expected.as_mut().unwrap().next = Some(Box::new(ListNode::new(2)));
        expected.as_mut().unwrap().next.as_mut().unwrap().next = Some(Box::new(ListNode::new(3)));
        expected.as_mut().unwrap().next.as_mut().unwrap().next.as_mut().unwrap().next = Some(Box::new(ListNode::new(4)));

        assert_eq!(merged, expected);
    }

    #[test]
    fn test_merge_two_lists_duplicates() {
        // Test merging lists with duplicate values
        let mut list1 = Some(Box::new(ListNode::new(1)));
        list1.as_mut().unwrap().next = Some(Box::new(ListNode::new(2)));

        let mut list2 = Some(Box::new(ListNode::new(2)));
        list2.as_mut().unwrap().next = Some(Box::new(ListNode::new(3)));

        let merged = merge_two_lists(list1, list2);

        let mut expected = Some(Box::new(ListNode::new(1)));
        expected.as_mut().unwrap().next = Some(Box::new(ListNode::new(2)));
        expected.as_mut().unwrap().next.as_mut().unwrap().next = Some(Box::new(ListNode::new(2)));
        expected.as_mut().unwrap().next.as_mut().unwrap().next.as_mut().unwrap().next = Some(Box::new(ListNode::new(3)));

        assert_eq!(merged, expected);
    }
    
    #[test]
    fn test_merge_two_lists_different_lengths() {
        // Test merging lists of different lengths
        let mut list1 = Some(Box::new(ListNode::new(1)));
        list1.as_mut().unwrap().next = Some(Box::new(ListNode::new(3)));
        list1.as_mut().unwrap().next.as_mut().unwrap().next = Some(Box::new(ListNode::new(5)));

        let mut list2 = Some(Box::new(ListNode::new(2)));
        list2.as_mut().unwrap().next = Some(Box::new(ListNode::new(4)));

        let merged = merge_two_lists(list1, list2);

        let mut expected = Some(Box::new(ListNode::new(1)));
        expected.as_mut().unwrap().next = Some(Box::new(ListNode::new(2)));
        expected.as_mut().unwrap().next.as_mut().unwrap().next = Some(Box::new(ListNode::new(3)));
        expected.as_mut().unwrap().next.as_mut().unwrap().next.as_mut().unwrap().next = Some(Box::new(ListNode::new(4)));
        expected.as_mut().unwrap().next.as_mut().unwrap().next.as_mut().unwrap().next.as_mut().unwrap().next = Some(Box::new(ListNode::new(5)));

        assert_eq!(merged, expected);
    }
    
    #[test]
    fn test_merge_two_lists_single_elements() {
        // Test merging two single-element lists
        let list1 = Some(Box::new(ListNode::new(1)));
        let list2 = Some(Box::new(ListNode::new(2)));

        let merged = merge_two_lists(list1, list2);

        let mut expected = Some(Box::new(ListNode::new(1)));
        expected.as_mut().unwrap().next = Some(Box::new(ListNode::new(2)));

        assert_eq!(merged, expected);
    }
    
    #[test]
    fn test_merge_two_lists_negative_values() {
        // Test merging lists with negative values
        let mut list1 = Some(Box::new(ListNode::new(-3)));
        list1.as_mut().unwrap().next = Some(Box::new(ListNode::new(1)));

        let mut list2 = Some(Box::new(ListNode::new(-2)));
        list2.as_mut().unwrap().next = Some(Box::new(ListNode::new(5)));

        let merged = merge_two_lists(list1, list2);

        let mut expected = Some(Box::new(ListNode::new(-3)));
        expected.as_mut().unwrap().next = Some(Box::new(ListNode::new(-2)));
        expected.as_mut().unwrap().next.as_mut().unwrap().next = Some(Box::new(ListNode::new(1)));
        expected.as_mut().unwrap().next.as_mut().unwrap().next.as_mut().unwrap().next = Some(Box::new(ListNode::new(5)));

        assert_eq!(merged, expected);
    }
}

