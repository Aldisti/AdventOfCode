
let filename = "input.txt"

let get_int s : int =
  String.sub s 1 ((String.length s) - 1) |> int_of_string

let parse (s: string) : (char * int) =
  match String.get s 0 with
  | 'L' as e -> (e, (- get_int s))
  | 'R' as e -> (e, get_int s)
  | _ -> raise @@ Failure ("Invalid direction: " ^ s)

let rec modulus (n: int) (m: int) : int =
  if n < 0 then modulus (n + m) m
  else n mod m

let () =
  let lines = In_channel.with_open_text filename In_channel.input_lines in
  (* List.iter print_endline lines; *)
  let (rem, count) = List.map parse lines
    |> List.fold_left (fun (init, acc) (_, x) -> (
      let sum = modulus (init + x) 100 in
      (* Printf.printf "%d + (%d) %% = %d\n" init x sum; *)
      if sum = 0 then (sum, acc + 1)
      else (sum, acc)
    )) (50, 0)
  in
  Printf.printf "Remainder %d Count %d\n" rem count

