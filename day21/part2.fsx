
let rec play2 p1 p2 pp1 pp2 p1w p2w u =
   if pp2 >= 21 then p2w := !p2w + u
   else
     let move p rsum = (p + rsum - 1) % 10 + 1
     play2 p2 (move p1 6) pp2 (pp1 + move p1 6) p2w p1w (u*7UL)
     play2 p2 (move p1 5) pp2 (pp1 + move p1 5) p2w p1w (u*6UL)
     play2 p2 (move p1 7) pp2 (pp1 + move p1 7) p2w p1w (u*6UL)
     play2 p2 (move p1 8) pp2 (pp1 + move p1 8) p2w p1w (u*3UL)
     play2 p2 (move p1 4) pp2 (pp1 + move p1 4) p2w p1w (u*3UL)
     play2 p2 (move p1 3) pp2 (pp1 + move p1 3) p2w p1w u
     play2 p2 (move p1 9) pp2 (pp1 + move p1 9) p2w p1w u


let args = System.Environment.GetCommandLineArgs()
let sp1, sp2 = int args.[1], int args.[2]

let p1w = ref 0UL
let p2w = ref 0UL

play2 sp1 sp2 0 0 p1w p2w 1UL

printfn "%d" <| max !p1w !p2w
