
let filename = "input.txt"

let print_tuple (a, b) =
  Printf.printf "(%d, %d)\n" a b

let to_range s : (int * int) =
  match String.split_on_char '-' s with
  | s :: e :: _ -> (int_of_string s, int_of_string e)
  | _ -> (0, 0)

let is_valid (n: int) : bool =
  let num = string_of_int n in
  let length = String.length num in
  let seq_size = length / 2 in
  if length mod 2 = 1 then true
  else String.(
    sub num 0 seq_size <> sub num seq_size seq_size
  )

let () =
  In_channel.with_open_text filename In_channel.input_all
  |> String.split_on_char ','
  |> List.filter (fun s -> s <> "")
  |> List.map to_range
  |> List.fold_left (fun acc (s, e) ->
    acc + (Seq.init (e - s + 1) (fun offset -> s + offset)
    |> Seq.fold_left (fun acc n ->
      if is_valid n then acc else acc + n
    ) 0)
  ) 0
  |> Printf.printf "%d\n"
