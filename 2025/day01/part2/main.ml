
let filename = "input.txt"

let get_int s : int =
  String.sub s 1 ((String.length s) - 1) |> int_of_string

let parse (s: string) : (char * int) =
  match String.get s 0 with
  | 'L' as e -> (e, (- get_int s))
  | 'R' as e -> (e, get_int s)
  | _ -> failwith ("Invalid direction: " ^ s)

let modulus (num, add) (m: int) : (int * int) =
  let (inc, add) = if add < 0 then (-1, -add) else (1, add) in
  let rec inner acc n i =
    if i >= add then (n, acc)
    else (
      let sum = (n + inc + m) mod m in 
      inner (acc + if sum = 0 then 1 else 0) (sum) (i + 1)
    )
  in
  inner 0 num 0

let () =
  let lines = In_channel.with_open_text filename In_channel.input_lines in
  let (rem, count) = List.map parse lines
    |> List.fold_left (fun (init, acc) (_, x) -> (
      let (sum, loops) = modulus (init, x) 100 in
      (sum, acc + loops)
    )) (50, 0)
  in
  Printf.printf "Remainder %d Count %d\n" rem count
