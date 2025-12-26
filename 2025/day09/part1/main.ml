
let filename = "input.txt"

let to_pair s =
  match String.split_on_char ',' s with
  | left :: right :: _ -> (int_of_string left, int_of_string right)
  | [] | _ :: [] -> failwith ("Cannot convert to pair" ^ s)

let calc_area (ax, ay) (bx, by) : int =
  ((ax - bx |> abs) + 1) * ((ay - by |> abs) + 1)

let foo pairs =
  let rec inner (mx: int) e = function
  | [] -> mx
  | q :: t -> (
    let area = calc_area e q in
    let mx = max mx area in
    inner mx e t
  )
  in
  let rec bar mx = function
  | [] | _ :: [] -> mx
  | e :: t -> bar (max mx (inner mx e t)) t
  in
  bar 0 pairs

let () =
  In_channel.with_open_text filename In_channel.input_lines
  |> List.map to_pair
  |> foo
  |> print_int; print_newline ()
