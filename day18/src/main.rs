use std::fs::File;
use std::io::{BufReader, BufRead};

#[derive(Debug, Clone)]
enum SnailfishNumber {
    N(u64),
    P(Box<Pair>)
}

#[derive(Debug, Clone)]
struct Pair(SnailfishNumber, SnailfishNumber);

fn parse_num(s: &str) -> SnailfishNumber {
    if s.starts_with('[') {
        let mut open_brackets = 1;
        let mut comma = None;
        let mut start = 1;
        while open_brackets > 0 {
            start = start+s[start..].find(|c| c == ']' || c == '[' || c == ',').unwrap();

            match &s[start..start+1] {
                "]" => open_brackets -= 1,
                "[" => open_brackets += 1,
                "," if open_brackets == 1 => comma = Some(start),
                "," => (),
                _ => unreachable!(),
            }
            start += 1;
        }
        let comma = comma.unwrap();

        SnailfishNumber::P(Box::new(Pair(parse_num(&s[1..comma]), parse_num(&s[comma+1..s.len()-1]))))
    } else {
        SnailfishNumber::N(s.parse().unwrap())
    }
}

fn split(n: &mut SnailfishNumber) -> bool {
    match n {
        &mut SnailfishNumber::N(rn) => {
            if rn >= 10 {
                *n = SnailfishNumber::P(Box::new(Pair(SnailfishNumber::N(rn / 2), SnailfishNumber::N(rn / 2 + rn % 2))));
                true
            } else {
                false
            }
        },
        SnailfishNumber::P(p) => {
            let Pair(l, r) = &mut**p;
            split(l) || split(r)
        }
    }
}

fn add_right(n: &mut SnailfishNumber, rn: u64) -> bool {
    match n {
        SnailfishNumber::N(n) => {
            *n += rn;
            true
        },
        SnailfishNumber::P(p) => {
            let Pair(l, r) = &mut**p;

            add_right(l, rn) || add_right(r, rn)
        }
    }
}
fn add_left(n: &mut SnailfishNumber, rn: u64) -> bool {
    match n {
        SnailfishNumber::N(n) => {
            *n += rn;
            true
        },
        SnailfishNumber::P(p) => {
            let Pair(l, r) = &mut**p;

            add_left(r, rn) || add_left(l, rn)
        }
    }
}

fn explode(n: &mut SnailfishNumber, depth: u32) -> Option<(Option<u64>, Option<u64>)> {
    match n {
        SnailfishNumber::N(_) => None,
        SnailfishNumber::P(p) => {
            // I'm just glad that this works, please don't ask me how
            let Pair(l, r) = &mut**p;

            if depth >= 4 {
                match (l, r) {
                    (&mut SnailfishNumber::N(a), &mut SnailfishNumber::N(b)) => {
                        *n = SnailfishNumber::N(0);
                        Some((Some(a), Some(b)))
                    },
                    _ => unimplemented!(),
                }
            } else {
                let res = explode(l, depth+1);
                if let Some((al, mut ar)) = res {
                    if let Some(rn) = ar {
                        if add_right(r, rn) {
                            ar = None;
                        }
                    }
                    Some((al, ar))
                } else {
                    let res = explode(r, depth+1);
                    if let Some((mut al, ar)) = res {
                        if let Some(rn) = al {
                            if add_left(l, rn) {
                                al = None;
                            }
                        }
                        Some((al, ar))
                    } else {
                        None
                    }
                }
            }
        }
    }
}

fn reduce(mut n: SnailfishNumber) -> SnailfishNumber {
    loop {
        if explode(&mut n, 0).is_none() {
            if !split(&mut n) {
                break;
            }
        }
    }

    n
}

fn add(l: SnailfishNumber, r: SnailfishNumber) -> SnailfishNumber {
    reduce(SnailfishNumber::P(Box::new(Pair(l, r))))
}

fn magnitude(n: &SnailfishNumber) -> u64 {
    match n {
        &SnailfishNumber::N(n) => n,
        &SnailfishNumber::P(ref p) => {
            let Pair(l, r) = &**p;
            3 * magnitude(l) + 2 * magnitude(r)
        }
    }
}

fn main() {
    let f = BufReader::new(File::open("input.txt").unwrap());
    let mut v = Vec::new();

    for line in f.lines() {
        let line = line.unwrap();

        v.push(parse_num(&line));
    }

    let sum = v.iter().cloned()
        .reduce(|a, b| add(a, b))
        .unwrap();
    println!("{}", magnitude(&sum));

    let mut largest = 0;

    for l in &v {
        for r in &v {
            let sum = magnitude(&add(l.clone(), r.clone()));
            if sum > largest {
                largest = sum;
            }
        }
    }

    println!("{}", largest);
}
