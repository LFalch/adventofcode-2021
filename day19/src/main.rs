use std::fs::File;
use std::io::{BufReader, BufRead};
use std::collections::HashSet;
use std::ops::{Add, Sub};

impl Add for Position {
    type Output = Self;
    fn add(self, rhs: Self) -> Self::Output {
        Position(self.0+rhs.0, self.1+rhs.1, self.2+rhs.2)
    }
}
impl Sub for Position {
    type Output = Self;
    fn sub(self, rhs: Self) -> Self::Output {
        Position(self.0-rhs.0, self.1-rhs.1, self.2-rhs.2)
    }
}

#[derive(Debug, Clone, Copy, PartialOrd, Ord, PartialEq, Eq, Hash)]
struct Position(i64, i64, i64);

impl Position {
    fn manhatten_length(&self) -> i64 {
        self.0.abs() + self.1.abs() + self.2.abs()
    }
}

fn orient(p: Position, o: u8) -> Position {
    let up = o & 3;
    let forwards = o >> 2;

    match (forwards, up) {
        (0, 0) => Position(p.0, p.1, p.2),
        (0, 1) => Position(p.0, -p.2, p.1),
        (0, 2) => Position(p.0, -p.1, -p.2),
        (0, 3) => Position(p.0, p.2, -p.1),
        (1, 0) => Position(-p.0, -p.1, p.2),
        (1, 1) => Position(-p.0, -p.2, -p.1),
        (1, 2) => Position(-p.0, p.1, -p.2),
        (1, 3) => Position(-p.0, p.2, p.1),
        (2, 0) => Position(p.1, p.2, p.0),
        (2, 1) => Position(p.1, -p.0, p.2),
        (2, 2) => Position(p.1, -p.2, -p.0),
        (2, 3) => Position(p.1, p.0, -p.2),
        (3, 0) => Position(-p.1, -p.2, p.0),
        (3, 1) => Position(-p.1, -p.0, -p.2),
        (3, 2) => Position(-p.1, p.2, -p.0),
        (3, 3) => Position(-p.1, p.0, p.2),
        (4, 0) => Position(p.2, p.0, p.1),
        (4, 1) => Position(p.2, -p.1, p.0),
        (4, 2) => Position(p.2, -p.0, -p.1),
        (4, 3) => Position(p.2, p.1, -p.0),
        (5, 0) => Position(-p.2, -p.0, p.1),
        (5, 1) => Position(-p.2, -p.1, -p.0),
        (5, 2) => Position(-p.2, p.0, -p.1),
        (5, 3) => Position(-p.2, p.1, p.0),
        _ => unreachable!(),
    }
}

fn main() {
    let input = BufReader::new(File::open("input.txt").unwrap()).lines();

    let mut scanners = Vec::new();

    for line in input {
        let line = line.unwrap();

        if line.starts_with("--- scanner ") {
            scanners.push(HashSet::new());
        } else if !line.is_empty() {
            let scanner = scanners.last_mut().unwrap();

            let mut coords = line.split(',').map(|s| s.parse().unwrap());
            let p = Position(coords.next().unwrap(), coords.next().unwrap(), coords.next().unwrap());
            assert!(coords.next().is_none());
            scanner.insert(p);
        }
    }

    assert!((0..24).any(|o| orient(Position(5,6,-4), o) == Position(5,6,-4)));
    assert!((0..24).any(|o| orient(Position(5,6,-4), o) == Position(-5,4,-6)));
    assert!((0..24).any(|o| orient(Position(5,6,-4), o) == Position(4,6,5)));
    assert!((0..24).any(|o| orient(Position(5,6,-4), o) == Position(-4,-6,5)));
    assert!((0..24).any(|o| orient(Position(5,6,-4), o) == Position(-6,-4,-5)));

    let mut points = scanners.remove(0);
    let mut scanner_positions = vec![Position(0, 0, 0)];

    loop {
        let mut residual = Vec::new();

        for scanner in scanners {
            let mut best_match = None;
            let mut highest_matching = 0;

            for fix in &points {
                for o in 0..24 {
                    for fix_match in scanner.iter().map(|&p| orient(p, o)) {
                        let off = *fix - fix_match;

                        let adjusted_points = scanner
                            .iter()
                            .map(move |&p| orient(p, o)+off);

                        let matching = adjusted_points.clone().filter(|p| points.contains(p)).count();
                        if matching > highest_matching {
                            highest_matching = matching;
                            best_match = Some((adjusted_points, off));
                        }
                    }
                }
            }

            if highest_matching >= 12 {
                let (iter, off) = best_match.unwrap();
                points.extend(iter);
                scanner_positions.push(off);
            } else {
                residual.push(scanner);
            }
        }

        if residual.is_empty() {
            break;
        } else {
            eprintln!("{} left", residual.len());
            scanners = residual;
        }
    }

    println!("{}", points.len());

    let mut biggest_distance = 0;

    for &a in &scanner_positions {
        for &b in &scanner_positions {
            let dist = (a-b).manhatten_length();

            if dist > biggest_distance {
                biggest_distance = dist;
            }
        }
    }

    println!("{}", biggest_distance);
}
