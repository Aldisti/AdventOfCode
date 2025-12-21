
let filename = "input.txt"

let is_range s = String.contains s '-'

let get_range s : (int * int) =
  match String.split_on_char '-' s |> List.map int_of_string with
  | a :: b :: _ -> (a, b)
  | _ -> (0, 0)

let check_overlap (al, ar) (bl, br) : (int * int) option =
  if al <= bl && ar >= br then Some (al, ar)
  else if al >= bl && al <= br + 1 then Some (bl, ar)
  else None

let merge_ranges ranges =
  let rec inner acc = function
    | [] -> acc
    | e :: [] -> e :: acc
    | e :: h :: t -> (
      match check_overlap e h with
      | Some m -> inner acc (m :: t)
      | None -> inner (e :: acc) (h :: t)
    )
  in
  inner [] ranges

let compare_range (al, ar) (bl, br) : int =
  if ar <> br then
    br - ar
  else
    bl - al

let () =
  In_channel.with_open_text filename In_channel.input_lines
  |> List.take_while is_range
  |> List.map get_range
  |> List.sort compare_range
  |> merge_ranges
  |> List.fold_left (fun acc (s, e) -> acc + (e - s + 1)) 0
  |> print_int; print_newline () 

