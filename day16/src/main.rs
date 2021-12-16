use std::fs::File;
use std::io::{BufReader, Read};

struct BitStream<I> {
    inner: I,
    byte: u8,
    bits_left: u8,
}

impl<I> BitStream<I> {
    fn new(inner: I) -> Self {
        BitStream{
            inner,
            byte: 0,
            bits_left: 0,
        }
    }
    fn next_u8(&mut self) -> u8 where Self: Iterator<Item=bool> {
        self.next().unwrap() as u8
    }
}

impl Iterator for BitStream<std::vec::IntoIter<bool>> {
    type Item = bool;

    fn next(&mut self) -> Option<Self::Item> {
        self.inner.next()
    }
}

impl<R: Read> Iterator for BitStream<BufReader<R>> {
    type Item = bool;

    fn next(&mut self) -> Option<Self::Item> {
        if self.bits_left == 0 {
            let mut buf = [0; 2];
            if self.inner.read(&mut buf).unwrap() != 2 {
                return None;
            }
            self.byte = u8::from_str_radix(std::str::from_utf8(&buf).unwrap(), 16).unwrap();
            self.bits_left += 8;
        }

        let bit = self.byte & 0x80 != 0;
        self.byte <<= 1;
        self.bits_left -= 1;

        Some(bit)
    }
}

fn number_3bit<R>(bs: &mut BitStream<R>) -> u8 where BitStream<R>: Iterator<Item=bool> {
    (bs.next_u8() << 2) | (bs.next_u8() << 1) | bs.next_u8()
}
fn number_4bit<R>(bs: &mut BitStream<R>) -> u8 where BitStream<R>: Iterator<Item=bool> {
    (bs.next_u8() << 3) |(bs.next_u8() << 2) | (bs.next_u8() << 1) | bs.next_u8()
}

fn lead_4bit<R>(bs: &mut BitStream<R>) -> (bool, u8) where BitStream<R>: Iterator<Item=bool> {
    (bs.next().unwrap(), number_4bit(bs))
}

fn nbit<R>(bs: &mut BitStream<R>, n: u8) -> u16 where BitStream<R>: Iterator<Item=bool> {
    debug_assert!(n <= 16);
    let mut number = 0;

    for _ in 0..n {
        number = (number << 1) | (bs.next_u8() as u16);
    }

    number
}

#[derive(Debug, Clone)]
struct Packet {
    version: u8,
    body: PacketBody
}

#[derive(Debug, Clone)]
enum PacketBody {
    Literal(u128),
    Operator(u8, Vec<Packet>),
}

fn parse_packet<I>(bs: &mut BitStream<I>) -> Packet where BitStream<I>: Iterator<Item=bool> {
    let version = number_3bit(bs);
    let id = number_3bit(bs);

    let body = match id {
        4 => {
            let mut cont = true;
            let mut number = 0u128;

            while cont {
                let (lead, nibble) = lead_4bit(bs);
                cont = lead;

                number = (number << 4) | (nibble as u128);
            }

            PacketBody::Literal(number)
        }
        id => {
            let length_type_id = bs.next().unwrap();
            let mut ops = Vec::new();

            if length_type_id {
                // number of packets
                let num_packets = nbit(bs, 11);

                for _ in 0..num_packets {
                    ops.push(parse_packet(bs));
                }

                PacketBody::Operator(id, ops)
            } else {
                // sub packets length
                let length = nbit(bs, 15);
                let mut v = Vec::with_capacity(length as usize);

                for _ in 0..length {
                    v.push(bs.next().unwrap());
                }

                let mut new_bs = BitStream::new(v.into_iter());
                while new_bs.inner.len() > 0 {
                    ops.push(parse_packet::<std::vec::IntoIter<bool>>(&mut new_bs));
                }

                PacketBody::Operator(id, ops)
            }
        }
    };

    Packet {
        version,
        body
    }
}

fn version_sum(packet: &Packet) -> u64 {
    let version = packet.version as u64;
    match packet.body {
        PacketBody::Literal(_) => version,
        PacketBody::Operator(_, ref ops) => {
            ops.iter().map(version_sum).chain(Some(version)).sum()
        }
    }
}

fn evaluate(packet: &Packet) -> u128 {
    match packet.body {
        PacketBody::Literal(n) => n,
        PacketBody::Operator(id, ref ops) => {
            match id {
                0 => ops.iter().map(evaluate).sum(),
                1 => ops.iter().map(evaluate).product(),
                2 => ops.iter().map(evaluate).min().unwrap(),
                3 => ops.iter().map(evaluate).max().unwrap(),
                5 => (evaluate(&ops[0]) > evaluate(&ops[1])) as u128,
                6 => (evaluate(&ops[0]) < evaluate(&ops[1])) as u128,
                7 => (evaluate(&ops[0]) == evaluate(&ops[1])) as u128,
                _ => unimplemented!(),
            }
        }
    }
}

fn main() {
    let f = BufReader::new(File::open("input.txt").unwrap());

    let packet = parse_packet(&mut BitStream::new(f));
    println!("{:?}", &packet);

    let vs = version_sum(&packet);

    println!("{}", vs);

    println!("{}", evaluate(&packet));
}
