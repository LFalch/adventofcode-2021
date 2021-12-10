use std::{
    fs::File,
    io::{BufRead, BufReader},
};

fn part1_points(b: u8) -> u64 {
    match b {
        b')' => 3,
        b']' => 57,
        b'}' => 1197,
        b'>' => 25137,
        _ => unreachable!(),
    }
}
fn part2_points(b: u8) -> u64 {
    match b {
        b')' => 1,
        b']' => 2,
        b'}' => 3,
        b'>' => 4,
        _ => unreachable!(),
    }
}

fn main() {
    let f = File::open("input.txt").unwrap();
    let (p1, p2) = BufReader::new(f).lines().map(|line| {
        line.unwrap().bytes()
            .try_fold(Vec::new(), |mut v, b| {
                match b {
                    b'(' => v.push(b')'),
                    b'[' => v.push(b']'),
                    b'{' => v.push(b'}'),
                    b'<' => v.push(b'>'),
                    b')' | b']' | b'}' | b'>' => {
                        if v.pop() != Some(b) {
                            return Err(b)
                        }
                    }
                    _ => unreachable!(),
                }
                Ok(v)
            })
            .map_err(part1_points)
            .map(|v| v.into_iter().rev().map(part2_points).fold(0, |acc, p| acc * 5 + p))
    })
    .fold((0, Vec::new()), |(p1, mut p2_vec), r| {
        match r {
            Err(p) => (p1+p, p2_vec),
            Ok(p) => {
                let i = p2_vec.binary_search(&p).unwrap_or_else(|e| e);
                p2_vec.insert(i, p);
                (p1, p2_vec)
            }
        }
    });
    println!("{}", p1);
    println!("{}", p2[p2.len()/2]);
}
