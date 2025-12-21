
let filename = "input.txt"
let separator = ' '


let split_on_spaces s =
  String.split_on_char ' ' s
  |> List.filter_map (fun s ->
    match String.trim s with
    | "" -> None
    | e -> Some e
  )

let rev_map f l =
  let rec inner acc = function
  | [] -> acc
  | e :: t -> inner ((f e) :: acc) t
  in
  inner [] l

let rotate (l: string list list) =
  let rec inner acc l =
    if List.(is_empty l || is_empty @@ hd l) then acc
    else inner (List.map List.hd l :: acc) (List.map List.tl l)
  in
  inner [] l

let fold_and_calculate l : int =
  match l with
  | [] -> 0
  | sign :: t -> (
    let t = List.map int_of_string t in
    match sign with
    | "+" -> List.fold_left ( + ) 0 t
    | "*" -> List.fold_left ( * ) 1 t
    | _ -> failwith ("Invalid operator: " ^ sign))

let () =
  In_channel.with_open_text filename In_channel.input_lines
  |> rev_map split_on_spaces
  |> rotate
  |> List.map fold_and_calculate
  |> List.fold_left (+) 0
  |> string_of_int |> print_endline

