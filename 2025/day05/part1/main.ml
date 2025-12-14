
let filename = "input.txt"

let is_range s = String.contains s '-'

let get_range s : (int * int) =
  match String.split_on_char '-' s |> List.map int_of_string with
  | a :: b :: _ -> (a, b)
  | _ -> (0, 0)

let is_in_range (n: int) (s, e) : bool =
  n >= s && n <= e

let rec is_in_any_range (n: int) ranges : bool =
  match ranges with
  | [] -> false
  | e :: t -> if is_in_range n e then true
    else is_in_any_range n t

let partition s =
  if String.contains s '-' then
    Either.left (get_range s)
  else
    Either.right s

let () =
  let (ranges, nums) =
    In_channel.with_open_text filename In_channel.input_lines
    |> List.partition_map partition
  in
  List.tl nums
  |> List.map int_of_string
  |> List.fold_left (fun acc x -> if is_in_any_range x ranges
    then acc + 1 else acc) 0
  |> print_int; print_newline ()
  
