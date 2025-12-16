use std::collections::VecDeque;

pub fn can_finish(num_courses: i32, prerequisites: Vec<Vec<i32>>) -> bool {
    let n = num_courses as usize;
    let mut q: VecDeque<usize> = VecDeque::with_capacity(n);
    let mut indegree = vec![0; n];
    let mut graph = vec![Vec::<usize>::new(); n];
    for pre in prerequisites {
        graph[pre[1] as usize].push(pre[0] as usize);
        indegree[pre[0] as usize] += 1;
    }
    for i in 0..n {
        if indegree[i] == 0 {
            q.push_back(i);
        }
    }
    let mut count = 0usize;
    while let Some(course) = q.pop_front() {
        count += 1;
        for &next_course in &graph[course] {
            indegree[next_course] -= 1;
            if indegree[next_course] == 0 {
                q.push_back(next_course);
            }
        }
    }
    count == n
}

// pub fn find_order(num_courses: i32, prerequisites: Vec<Vec<i32>>) -> Vec<i32> {
        
// }