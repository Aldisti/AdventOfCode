
let filename = "input.txt"

let print_tuple (a, b) =
  Printf.printf "(%d, %d)\n" a b

let to_range s : (int * int) =
  match String.split_on_char '-' s with
  | s :: e :: _ -> (int_of_string s, int_of_string e)
  | _ -> (0, 0)

let split_chunks s size : string list =
  let len = String.length s in
  let rec inner pos acc =
    if pos >= len then acc
    else inner (pos + size) (String.sub s pos size :: acc)
  in
  if len mod size <> 0 then []
  else inner 0 []

let is_valid (n: int) : bool =
  let num = string_of_int n in
  let len = String.length num in
  let rec inner i =
    if i > len / 2 then true else (
      match split_chunks num i with
      | [] -> inner (i + 1)
      | e :: t -> (
        if List.for_all (fun x -> x = e) t then false
        else inner (i + 1)
      )
    )
  in
inner 1

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
