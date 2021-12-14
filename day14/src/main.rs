use std::fs::File;
use std::io::{BufRead, BufReader, Read};
use std::collections::HashMap;

pub fn print_rc(rule_count: &HashMap<[u8; 2], u64>) {
    for (r, n) in rule_count {
        println!("{}: {}", std::str::from_utf8(r).unwrap(), n);
    }
}

fn min_max_char(rule_count: &HashMap<[u8; 2], u64>) -> (u64, u64) {
    let mut occs = HashMap::new();

    for ([r1, r2], n) in rule_count {
        *occs.entry(r1).or_insert(0) += n;
        *occs.entry(r2).or_insert(0) += n;
    }

    // We are double-counting each character, so we divide by 2
    let min = *occs.values().min().unwrap() / 2;
    let max = *occs.values().max().unwrap() / 2;

    (min, max)
}

fn step_rule_count(rule_count: HashMap<[u8; 2], u64>, rules: &HashMap<[u8; 2], u8>) -> HashMap<[u8; 2], u64> {
    let mut rule_count2 = HashMap::new();
    for (rule, n) in rule_count {
        let mid = rules[&rule];
        let [r1, r2] = rule;

        *rule_count2.entry([r1, mid]).or_insert(0) += n;
        *rule_count2.entry([mid, r2]).or_insert(0) += n;
    }
    rule_count2
}

fn main() {
    let mut f = BufReader::new(File::open("input.txt").unwrap());

    let mut s = String::new();
    f.read_line(&mut s).unwrap();
    s.pop();

    f.read(&mut [b'\n']).unwrap();

    let mut rules = HashMap::new();

    for line in f.lines() {
        let line = line.unwrap();
        let line = line.as_bytes();

        let rule: [u8; 2] = [line[0], line[1]];
        let result = line[6];

        rules.insert(rule, result);
    }

    let mut rule_count = HashMap::new();
     for rule in s.as_bytes().windows(2) {
        let rule = [rule[0], rule[1]];
        *rule_count.entry(rule).or_insert(0) += 1u64;
    }

    for _ in 0..10 {
        rule_count = step_rule_count(rule_count, &rules);
    }

    let (min, max) = min_max_char(&rule_count);

    println!("{}", max-min);

    for _ in 10..40 {
        rule_count = step_rule_count(rule_count, &rules);
    }

    let (min, max) = min_max_char(&rule_count);

    println!("{}", max-min);
}
