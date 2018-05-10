:paste

val split_loop : (List[List[Int]], List[Int], List[Int], Int => Boolean) => List[List[Int]] = {
    case (result, Nil ,  Nil,   _)  => result
    case (result, accum, Nil,   _)  => result:::List(accum)
    case (result, accum, x::xs, p) if (p(x)) => split_loop(result:::List(accum), Nil, xs, p)
    case (result, accum, x::xs, p)           => split_loop(result, accum:::List(x), xs, p)
}

val split : (List[Int], Int => Boolean) => List[List[Int]] = (list, p) => split_loop(Nil, Nil, list, p)


val list   = List (1 , 2 , 0 , -5 , 0 , -100)
val result = split ( list , _ == 0)

val list2    = Nil
val result2  = split ( list2 , _ == 0)

val list3    = List(0, 0, 0)
val result3  = split ( list3 , _ == 0)

