use std::{
    fs::File,
    // str::FromStr,
    fmt::{self, Debug},
    collections::{HashMap, BTreeMap}, marker::PhantomData,
};

use serde::{Deserialize, Deserializer, de::{Visitor, Error, Unexpected}};
use chrono::{DateTime, Local, TimeZone};

struct StringOrIntDeserializer<T> {
    _phant: PhantomData<T>,
}

impl<T> Default for StringOrIntDeserializer<T> {
    fn default() -> Self {
        StringOrIntDeserializer { _phant: PhantomData }
    }
}

macro_rules! impl_string_or_int {
    ($($t:ident, $visit:ident,)*) => {
        $(
impl<'de> Visitor<'de> for StringOrIntDeserializer<$t> {
    type Value = $t;

    fn expecting(&self, formatter: &mut fmt::Formatter) -> fmt::Result {
        write!(formatter, "a {} or a string that can be parsed to {0}", stringify!($ty))
    }

    fn $visit<E>(self, v: $t) -> Result<Self::Value, E>
    where E: Error,
    {
        Ok(v)
    }

    fn visit_str<E>(self, v: &str) -> Result<Self::Value, E>
    where E: Error,
    {
        match v.parse() {
            Ok(v) => Ok(v),
            Err(_) => {
                Err(Error::invalid_value(Unexpected::Str(v), &concat!("a ", stringify!($ty), " in a string")))
            }
        }
    }
}
        )*
    };
}

impl_string_or_int!{
    u16, visit_u16,
    u32, visit_u32,
    u64, visit_u64,
}

fn from_maybe_str<'de, T, D: Deserializer<'de>>(d: D) -> Result<<StringOrIntDeserializer<T> as Visitor<'de>>::Value, D::Error>
where StringOrIntDeserializer<T>: Visitor<'de> {
    d.deserialize_any(StringOrIntDeserializer::default())
}

fn from_timestmap<'de, D: Deserializer<'de>>(d: D) -> Result<DateTime<Local>, D::Error> {
    let ts = d.deserialize_any(StringOrIntDeserializer::<u64>::default())?;

    Ok(Local.timestamp(ts as i64, 0))
}

#[derive(Debug, Deserialize)]
struct Leaderboard {
    members: HashMap<u32, Member>,
    #[serde(deserialize_with = "self::from_maybe_str::<u16, _>")]
    event: u16,
    #[serde(deserialize_with = "self::from_maybe_str::<u32, _>")]
    owner_id: u32,
}

#[derive(Debug, Deserialize)]
struct Member {
    global_score: u16,
    name: String,
    completion_day_level: BTreeMap<u8, Day>,
    stars: u16,
    #[serde(deserialize_with = "self::from_maybe_str::<u32, _>")]
    id: u32,
    #[serde(deserialize_with = "from_timestmap")]
    last_star_ts: DateTime<Local>,
    local_score: u16,
}

#[derive(Debug, Deserialize)]
struct Day {
    #[serde(rename = "1")]
    part1: Option<StarTs>,
    #[serde(rename = "2")]
    part2: Option<StarTs>,
}
#[derive(Debug, Deserialize)]
struct StarTs {
    #[serde(deserialize_with = "from_timestmap")]
    get_star_ts: DateTime<Local>,
}

fn star_gets(member: &Member) {
       let mut star_gets: Vec<_> = member.completion_day_level.iter().flat_map(|(&d, day)| {
        match (&day.part1, &day.part2) {
            (None, None) => vec![].into_iter(),
            (Some(p1), Some(p2)) => vec![(d, 1, p1.get_star_ts.clone()), (d, 2, p2.get_star_ts.clone())].into_iter(),
            (Some(p1), None) => vec![(d, 1u8, p1.get_star_ts.clone())].into_iter(),
            (None, Some(p2)) => vec![(d, 2, p2.get_star_ts.clone())].into_iter(),
        }
    }).collect();
    star_gets.sort_by_key(|&(d, p, _)| (d, p));

    println!("Last star of {}: {}", member.name, member.last_star_ts);

    for (d, p, dt) in star_gets {
        println!("{:2}p{} @ {}", d, p, dt);
    }
}

fn main() {
    let l: Leaderboard = serde_json::from_reader(
        File::open("test.json").unwrap()
    ).unwrap();
    let owner = &l.members[&l.owner_id];

    star_gets(owner);
    println!();

    if let Some(due) = l.members.values().find(|m| m.name == "Erk") {
        star_gets(due);
    }
}
