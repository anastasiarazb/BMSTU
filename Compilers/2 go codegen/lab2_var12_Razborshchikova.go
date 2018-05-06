package main

import (
	"fmt"
	"go/ast"
	"go/format"
	"go/parser"
	//"go/printer"
	"go/token"
	"os"
)

func addDefaultMapCapacity(file *ast.File, capacity int) {
    ast.Inspect(file, func(node ast.Node) bool {
        if expr, isExpr := node.(*ast.CallExpr); isExpr {
            if fun, isFun := expr.Fun.(*ast.Ident); isFun {
                if fun.Name == "make" {
                    if _, isMap := expr.Args[0].(*ast.MapType); isMap {
                        if len(expr.Args) < 2 {
                            expr.Args = append(expr.Args,
                                                &ast.BasicLit{
                                                    Kind:  token.INT,
                                                    Value: fmt.Sprintf("%d", capacity),
                                                })
                        }
                    }
                }
            }
        }
        return true
    })
}

func main() {
	if len(os.Args) != 2 {
		return
	}

	fset := token.NewFileSet()
	if file, err := parser.ParseFile(fset, os.Args[1], nil, parser.ParseComments); err == nil {
        
        addDefaultMapCapacity(file, 16) 

		if format.Node(os.Stdout, fset, file) != nil {
			fmt.Printf("Formatter error: %v\n", err)
		}
		//ast.Fprint(os.Stdout, fset, file, nil)
	} else {
		fmt.Printf("Errors in %s\n", os.Args[1])
	}
}
