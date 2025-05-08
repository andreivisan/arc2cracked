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
        Some(&l1), None => l1,
        None, Some(&l2) => l2,
        Some(&l1), Some(&l2) => {
            if l1 <= l2 {
                l1 = l1.next;
            } else {
                let temp = &l1.next
                l1.next = &l2;
                l2.next = temp
            }
        },
        None, None => break;
    }         
}
