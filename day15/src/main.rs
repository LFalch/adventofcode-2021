use std::fs::File;
use std::hash::Hash;
use std::io::{BufReader, Read};
use std::collections::{HashSet, HashMap};
use std::collections::hash_map::Entry::*;

pub type Unit = u32;

#[derive(Hash, Ord, Eq, PartialEq, PartialOrd, Debug, Clone)]
pub struct Point(pub Unit, pub Unit);

impl Point {
    pub fn neighbours<F: Fn(&Point) -> bool>(&self, in_bounds: F) -> std::vec::IntoIter<Point> {
        let mut poses = vec![Point(self.0 + 1, self.1), Point(self.0, self.1 + 1)];
        if self.0 > 0 {
            poses.push(Point(self.0 - 1, self.1));
        }
        if self.1 > 0 {
            poses.push(Point(self.0, self.1 - 1));
        }
        poses.retain(|p| in_bounds(p));

        poses.into_iter()
    }
}

fn main() {
    // Pass 2 as cmdline arg if part 2
    let part2 = std::env::args().nth(1).map(|s| &*s == "2").unwrap_or(false);
    let mut f = BufReader::new(File::open("input.txt").unwrap());

    let mut grid = [[255; 150]; 150];

    let mut dim = 0;
    let mut cur_x = 0;
    let mut cur_y = 0;

    let mut buf = [0];

    while f.read(&mut buf).unwrap() == 1 {
        let [c] = buf;
        if c == b'\n' {
            cur_x += 1;
            cur_y = 0;
        } else {
            grid[cur_x][cur_y] = c - b'0';

            if cur_x == 0 { dim += 1; }
            cur_y += 1;
        }
    }

    let mut visited = HashSet::new();
    let mut tentative_distances = HashMap::new();
    let mut certain_distances = HashMap::new();
    certain_distances.insert(Point(0, 0), 0);
    let mut cur = Point(0, 0);
    let edge = if part2 { dim * 5 } else { dim };
    let end = Point(edge-1, edge-1);

    loop {
        let cur_cost = certain_distances[&cur];
        for Point(nx, ny) in cur.neighbours(|&Point(x, y)| x < edge && y < edge).filter(|p| !visited.contains(p)) {
            let cost = (grid[(nx % dim) as usize][(ny % dim) as usize] as u32 + nx / dim + ny / dim - 1) % 9 + 1;
            let tentative_distance = cur_cost + cost;

            match tentative_distances.entry(Point(nx, ny)) {
                Vacant(v) => {v.insert(tentative_distance);}
                Occupied(mut o) => {
                    if *o.get() > tentative_distance {
                        o.insert(tentative_distance);
                    }
                },
            }
        }

        if cur == end {
            break;
        }

        visited.insert(cur);

        let next_node = tentative_distances
            .iter()
            .map(|(p, &n)|(p.clone(), n))
            .min_by_key(|&(_, n)| n)
            .unwrap();
        certain_distances.insert(next_node.0.clone(), next_node.1);
        tentative_distances.remove(&next_node.0);
        cur = next_node.0;
    }

    println!("{}", certain_distances[&end]);
}
