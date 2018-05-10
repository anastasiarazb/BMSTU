:paste

object Polynom {

    def listToString(lst: List[Int]): String = {
        def buildString(string: String, lst: List[Int], power: Int): String = (string, lst, power) match {
            case (result, Nil,     _)     => result
            case (string, 0::tail, power) => buildString(string, tail, power+1)
            case (  _   , x::tail, 0)     => buildString(x.toString, tail, 1)
            case (string, x::tail, 1)     => buildString(string + " + " + x.toString + "x", tail, 2)
            case (string, x::tail, power) => buildString(string + " + " + x.toString + "x^" + power.toString, tail, power+1)
        }
        buildString("", lst, 0)
    }
    
    def mapZip(src1: List[Int], src2: List[Int], op: (Int, Int) => Int): List[Int] = {
        def buildMapZip(accum: List[Int], src1: List[Int], src2: List[Int], op: (Int, Int) => Int): List[Int] =
                 (accum, src1, src2) match {
            case (result, Nil, Nil)      => result
            case (result, Nil, x::tail)     => buildMapZip(accum:::List(op(0, x)), Nil, tail, op)
            case (result, x::tail, Nil)     => buildMapZip(accum:::List(op(x, 0)), tail, Nil, op)
            case (result, x::tail1, y::tail2) => buildMapZip(accum:::List(op(x, y)), tail1, tail2, op)
        }
        buildMapZip(Nil, src1, src2, op)
    }
}

class Polynom(pcoefs : List[Int]) {
    val coefs = pcoefs
    
    def len () = coefs.length
    def unary_!() = new Polynom(this.coefs.zipWithIndex.map({case (a, i) => a * i}).drop(1))
    def binOp(a: Polynom, b: Polynom, op: (Int, Int) => Int) = new Polynom(
        a.coefs.zipAll(b.coefs, 0, 0).map({case (x, y) => op(x, y)})
    )
    def + (p: Polynom) = binOp(this, p, _ + _)
    def - (p: Polynom) = binOp(this, p, _ - _)
    override def toString(): String = this.coefs.zipWithIndex.filter({case (a, i) => a != 0})
                                                .map   ({case (a, 0) => a.toString
                                                         case (a, 1) if (a > 0) =>  "+" + a.toString + "x"
                                                         case (a, 1) => "+" + a.abs.toString + "x"
                                                         case (a, i) if (a > 0) => "-" + a.abs.toString +
                                                         "x^(" + i.toString + ")"}
                                                         case (a, i) => "-" + a.abs.toString +
                                                         "x^(" + i.toString + ")"})
                                                .foldLeft("")(_ + _)
}

var X = new Polynom(1::2::3::Nil)
var Y = new Polynom(9::0::1::(-1)::4::Nil)
var Zero = new Polynom(Nil)

var XminusY = X - Y
var XplusY = X + Y

var XplusZero = X + Zero

var Z = !Y
