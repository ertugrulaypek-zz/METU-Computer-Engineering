module Hw1 where

type Mapping = [(String, String, String)]
data AST = EmptyAST | ASTNode String AST AST deriving (Show, Read)

writeExpression :: (AST, Mapping) -> String
evaluateAST :: (AST, Mapping) -> (AST, String)
-- DO NOT MODIFY OR DELETE THE LINES ABOVE -- 
-- IMPLEMENT writeExpression and evaluateAST FUNCTION ACCORDING TO GIVEN SIGNATURES -- 

writeExpression (xs,[]) = (writeExpression2(xs,[]))
writeExpression (xs,ys)= "let " ++ (writeExpression2 (xs,ys))

writeExpression2 (xs,[]) = helper xs
writeExpression2 (xs,[(varName, varType, varValue)]) 
	| varType == "num" =  varName ++ "=" ++ varValue ++ " in " ++ (writeExpression2 (xs, []) )
	| otherwise =  varName ++ "=\"" ++ varValue ++ "\" in " ++ (writeExpression2 (xs, []) )
writeExpression2 (xs,(varName, varType, varValue):ys)
	|varType == "num" =  varName ++ "=" ++ varValue ++ ";" ++ (writeExpression2 (xs, ys))
	|otherwise =  varName ++ "=\"" ++ varValue ++ "\";" ++ (writeExpression2 (xs, ys))

helper EmptyAST = ""
helper (ASTNode str EmptyAST EmptyAST) = str
helper (ASTNode str left EmptyAST)
	| str == "len" = "(length " ++ (helper left) ++ ")"
	| str == "negate" ="(-" ++ (helper left) ++ ")"
	| str == "str" = "\"" ++ (helper left) ++ "\""
	| str == "num" = (helper left)
	| otherwise = "ERROR"
helper (ASTNode str left right)
	| str == "plus" = "(" ++ (helper left) ++ "+" ++ (helper right) ++ ")"
	| str == "times" = "(" ++ (helper left) ++ "*" ++ (helper right) ++ ")"
	| str == "cat" = "(" ++ (helper left) ++ "++" ++ (helper right) ++ ")"
	| otherwise = "ERROR2"

evaluateAST (xs,[]) = (xs,(runEvaluate xs))
evaluateAST (xs,ys) = (recreateTree (xs,ys), (runEvaluate (recreateTree (xs, ys)) ))


recreateTree ((ASTNode str EmptyAST EmptyAST),ys)
	| tuple == ("","") = (ASTNode str EmptyAST EmptyAST)
	| otherwise = (ASTNode (snd tuple) (ASTNode (fst tuple) EmptyAST EmptyAST) EmptyAST )
	where tuple = getValue (str, ys)
recreateTree ((ASTNode str left EmptyAST),ys) = (ASTNode str (recreateTree (left,ys)) EmptyAST)
recreateTree ((ASTNode str left right), ys) = (ASTNode str (recreateTree (left,ys)) (recreateTree (right,ys)) )


getValue (requestedVarName, [(varName, varType, varValue)])
	| requestedVarName == varName = (varValue, varType)
	| otherwise = ("","")
getValue (requestedVarName, (varName, varType, varValue):ys)
	| requestedVarName == varName = (varValue, varType)
	| otherwise = (getValue (requestedVarName, ys))



runEvaluate (ASTNode str left EmptyAST)
	| str == "len" = show (length (runString left ))
	| str == "negate" = show (-1 * (runInt left ) )
runEvaluate (ASTNode str left right)
	| str == "plus" = show ((runInt left) + (runInt right))
	| str == "times" = show ((runInt left) * (runInt right))
	| str == "cat" = (runString left) ++ (runString right)

runInt (ASTNode str left EmptyAST)
	| str == "num" = (getNum left)
	| str == "negate" = (-1 * (runInt left))
	| str == "len" = (length (runString left))
runInt (ASTNode str left right)
	| str == "plus" = ((runInt left) + (runInt right))
	| str == "times" = ((runInt left) * (runInt right))

runString (ASTNode str left EmptyAST)
	| str == "str" = (getStr left)
runString (ASTNode str left right)
	| str == "cat" = (runString left) ++ (runString right)



getNum (ASTNode str _ _) = read str :: Int
getStr (ASTNode str _ _) = str