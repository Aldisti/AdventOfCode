
let filename = "input.txt"

let rec best_single grt head tail =
  match tail with
  | [] -> grt
  | e :: t -> (
    let tmp = head * 10 + e in
    best_single (if tmp > grt then tmp else grt) head t
  )

let best_joltage (nums: int list) : int =
  let rec inner grt = function
  | [] -> grt
  | e :: t -> (
    let tmp = best_single grt e t in
    inner (if tmp > grt then tmp else grt) t
  )
  in
  inner 0 nums
  (* let x = inner 0 nums in *)
  (* Printf.printf "best_joltage of %s is %d\n" (List.map string_of_int nums |> List.fold_left (^) "") x; x *)

let () =
  In_channel.with_open_text filename In_channel.input_lines
  |> List.map (fun s -> String.to_seq s |> List.of_seq |> List.map (fun c -> (int_of_char c) - 48))
  |> List.fold_left (fun acc nums -> acc + (best_joltage nums)) 0
  |> print_int; print_newline ()
