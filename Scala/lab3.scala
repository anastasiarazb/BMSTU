:paste

class Pos private ( val prog: String , val offs: Int , val line: Int , val col: Int ) {
    def this ( prog: String ) = this ( prog , 0, 1, 1)
    def ch = if ( offs == prog.length ) -1 else prog ( offs )
    def inc = ch match {
        case '\n' => new Pos ( prog, offs + 1 , line + 1 , 1)
        case -1 => this
        case _  => new Pos ( prog, offs + 1 , line , col + 1)
    }
    override def toString = "(" + line + " , " + col + ")"
}

object DomainTags extends Enumeration {
    type Tag = Value
    val WHITESPACE, IDENT, NUMBER, KEYWORD, END_OF_PROGRAM, UNDEFINED = Value
}

import DomainTags._

class Scanner {
    def scan ( start: Pos, errors: List[String] ): (Tag, Pos, List[String]) =
        sys.error ("[ERROR]: undefined token at " + start)
}

object Scanner {
    def saveError(pos: Pos, tag: Tag, message: String, errors: List[String]): List[String] = 
        errors:::List("***[" + tag.toString + "] syntax error at " + pos + ": " + message + "***")
}

class Token ( val start : Pos , scanner : Scanner, errors: List[String] ) {
    val ( tag , follow, err_list ) = start.ch match {
        case -1 => ( END_OF_PROGRAM , start, errors )
        case _  => scanner.scan( start, errors )
    }

    def image = start.prog.substring( start.offs , follow.offs )
    def next = new Token ( follow , scanner, errors )
}

//-------------------------------------------------------------------------

trait Whitespaces extends Scanner {
    private def missWhitespace ( pos : Pos ): Pos = pos.ch match {
        case ' '  => missWhitespace ( pos.inc )
        case '\t' => missWhitespace ( pos.inc )
        case '\n' => missWhitespace ( pos.inc )
        case  _   => pos
    }

    override def scan ( start : Pos, errors: List[String] ) = {
        val follow = missWhitespace ( start )
        if ( start != follow ) ( WHITESPACE, follow, errors )
        else super.scan( start, errors )
    }
}

//Ключевые слова: «ON», «OFF», «∗∗».
trait Keywords extends Scanner {
    private def scanWord ( pos : Pos): Pos = pos.ch match {
        case 'N' => pos.inc
        case 'F' => pos.inc.ch match {
                case 'F' => pos.inc.inc
                case  _  => pos
        }
        case  _  => pos
    }
    
    private def scanStar ( pos : Pos ): Pos = pos.ch match {
        case '*' => pos.inc
        case  _  => pos
    }

    override def scan ( start : Pos, errors: List[String] ) = {
        val next   = start.inc
        val follow = start.ch match {
            case 'O' => scanWord(next)
            case '*' => scanStar(next)
            case  _  => next
        }
        if (next != follow) ( KEYWORD, follow, errors )
        else super.scan( start, errors )
    }
}

trait Numbers extends Scanner {
    private def iterateSymbol ( pos: Pos , symbol: Int): Pos = {
        if (pos.ch == symbol) iterateSymbol( pos.inc, symbol )
        else pos
    }
    
    private def switchRule ( pos : Pos ): Pos = pos.ch match {
        case '*' => pos.inc
        case '+' => iterateSymbol ( pos.inc, '+' )
        case '-' => iterateSymbol ( pos.inc, '-' )
        case  _  => pos
    }

    override def scan ( start: Pos, errors: List[String] ) = {
        val follow = switchRule ( start )
        if ( start != follow ) (NUMBER, follow, errors )
        else super.scan( start, errors )
    }
}

trait Idents extends Scanner {

    private def iterateSymbol ( pos: Pos): Pos = pos.ch match {
        case '*' => iterateOp(pos.inc)
        case '+' => iterateOp(pos.inc)
        case '-' => iterateOp(pos.inc)
        case ch  if (ch.toChar.isUpper) => iterateSymbol(pos.inc)
        case  _  => pos
    }
    
    private def iterateOp ( pos : Pos ): Pos = pos.ch match {
        case '*' => iterateOp(pos.inc)
        case '+' => iterateOp(pos.inc)
        case '-' => iterateOp(pos.inc)
        case  _  => pos
    }

    override def scan ( start : Pos, errors: List[String] ) = {
        if (start.ch.toChar.isUpper) {
            val follow = iterateSymbol(start.inc)
            if (start != follow) (IDENT, follow, errors)
        }
        else super.scan(start, errors)
    }
}

//-----------------------------------------------------

var t = new Token(
    new Pos("  ---+   "),
    new Scanner
        with Idents
        with Numbers
        with Keywords
        with Whitespaces,
    Nil
    )
while (t.tag != END_OF_PROGRAM) {
    println(t.tag.toString + " " + t.start + "-" + t.follow + ": " + t.image)
    t = t.next
}

t.err_list

/*
- Идентификаторы: последовательности заглавных латинских букв, за которыми
могут располагаться последовательности знаков «+», «−» и «∗».
- Числовые литералы: знак «∗» или последовательности, состоящие целиком либо из знаков
«+», либо из знаков «−».
- Ключевые слова: «ON», «OFF», «∗∗».
*/
