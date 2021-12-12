use std::{
    fs::File,
    io::{BufRead, BufReader},
    collections::{HashMap, HashSet},
};

fn path1(cur: &str, connections: &HashMap<String, HashSet<String>>, visited: HashSet<String>, n: &mut u64) {
    if cur == "end" {
        *n += 1;
        return
    }

    let cs = connections[cur].difference(&visited).cloned().collect::<Vec<_>>();

    for connection in cs {
        let mut visited = visited.clone();
        if connection.chars().all(|c| c.is_lowercase()) {
            visited.insert(connection.clone());
        }
        path1(&connection, connections, visited, n);
    }
}

fn path2(cur: &str, connections: &HashMap<String, HashSet<String>>, visited: HashSet<String>, double_small_visit: bool, n: &mut u64) {
    if cur == "end" {
        *n += 1;
        return
    }

    let cs = connections[cur].difference(&visited).cloned().collect::<Vec<_>>();

    for connection in cs {
        let mut visited = visited.clone();
        if connection.chars().all(|c| c.is_lowercase()) {
            visited.insert(connection.clone());
        }
        path2(&connection, connections, visited, double_small_visit, n);
    }
    if !double_small_visit {
        let cs = connections[cur].intersection(&visited).filter(|s| &***s != "start").cloned().collect::<Vec<_>>();

        for connection in cs.into_iter() {
            path2(&connection, connections, visited.clone(), true, n);
        }
    }
}

fn main() {
    let mut connections = HashMap::new();

    let f = File::open("input.txt").unwrap();
    for line in BufReader::new(f).lines() {
        let line = line.unwrap();
        let dash_i = line.find('-').unwrap();

        let n1 = &line[..dash_i];
        let n2 = &line[dash_i+1..];

        connections.entry(n1.to_string()).or_insert_with(|| HashSet::new()).insert(n2.to_string());
        connections.entry(n2.to_string()).or_insert_with(|| HashSet::new()).insert(n1.to_string());
    }

    let mut n = 0;
    path1("start", &connections, HashSet::from_iter(vec!["start".to_string()]), &mut n);
    println!("{}", n);
    n = 0;
    path2("start", &connections, HashSet::from_iter(vec!["start".to_string()]), false, &mut n);
    println!("{}", n);
}
