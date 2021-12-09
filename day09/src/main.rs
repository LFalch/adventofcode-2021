use std::{
    io::{BufRead, BufReader},
    fs::File, collections::HashSet, cmp::Reverse,
};

fn neighbours(i: usize, j: usize, line_length: usize) -> impl Iterator<Item = (usize, usize)> {
    let mut n = Vec::with_capacity(4);
    if let Some(i) = i.checked_sub(1) {
        n.push((i, j));
    }
    if let Some(j) = j.checked_sub(1) {
        n.push((i, j));
    }
    if i+1 < line_length {
        n.push((i+1, j));
    }
    if j+1 < line_length {
        n.push((i, j+1));
    }
    n.into_iter()
}

const fn coords(index: usize, line_length: usize) -> (usize, usize) {
    (index % line_length, index / line_length)
}
const fn index(i: usize, j: usize, line_length: usize) -> usize {
    i + j * line_length
}

#[cfg(test)]
fn index_coords(i: usize) {
    let (a, b) = coords(i, 100);
    assert_eq!(index(a, b, 100), i);
}
#[test]
fn test1() {
    index_coords(7);
    index_coords(104);
    index_coords(512);
    index_coords(700);
}

fn main() {
    let mut f = BufReader::new(File::open("input.txt").unwrap());

    let mut buf = String::with_capacity(101);
    let mut points = Vec::with_capacity(10000);
    let mut line_length = 0;
    
    while f.read_line(&mut buf).unwrap_or(0) > 0 {
        let line = buf.trim();
        line_length = line.len();
        points.extend(line.as_bytes().iter().map(|&c| c - b'0'));
        buf.clear();
    }

    let low_points: Vec<_> = points
        .iter()
        .enumerate()
        .map(|(i, &b)| (coords(i, line_length), b))
        .filter(|&((i, j), b)| {
            neighbours(i, j, line_length)
                .map(|(i, j)| points[index(i, j, line_length)])
                .all(|b_adj| b_adj > b)
        })
        .map(|(i, _)| i)
        .collect()
        ;
    println!("Part 1: {}", low_points.iter().map(|&(i, j)| 1+points[index(i, j, line_length)] as usize).sum::<usize>());

    let part2: usize = low_points
        .into_iter()
        .map(|(i, j)| {
            let mut to_check = vec![(i, j)];
            let mut in_bassin = HashSet::new();

            while let Some((i, j)) = to_check.pop() {
                if points.get(index(i, j, line_length)) != Some(&9) {
                    in_bassin.insert((i, j));

                    for coords in neighbours(i, j, line_length) {
                        if !to_check.contains(&coords) && !in_bassin.contains(&coords) {
                            to_check.push(coords)
                        }
                    }
                }
            }
            in_bassin.len()
        })
        .fold(Vec::new(), |mut maxes, size| {
            let i = maxes.binary_search_by_key(&Reverse(&size), Reverse).unwrap_or_else(|i| i);
            maxes.insert(i, size);
            if maxes.len() > 3 {
                unsafe { maxes.set_len(3); }
            }
            maxes
        })
        .into_iter()
        .product();
    println!("Part 2: {}", part2);
}
