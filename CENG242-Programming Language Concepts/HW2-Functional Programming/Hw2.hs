module Hw2 where

data ASTResult = ASTError String | ASTJust (String, String, Int) deriving (Show, Read)
data ASTDatum = ASTSimpleDatum String | ASTLetDatum String deriving (Show, Read)
data AST = EmptyAST | ASTNode ASTDatum AST AST deriving (Show, Read)

isNumber :: String -> Bool
eagerEvaluation :: AST -> ASTResult
normalEvaluation :: AST -> ASTResult
-- DO NOT MODIFY OR DELETE THE LINES ABOVE -- 
-- IMPLEMENT isNumber, eagerEvaluation and normalEvaluation FUNCTIONS ACCORDING TO GIVEN SIGNATURES -- 

isNumber [] = False
isNumber (x:xs) = (elem x ('-':['0'..'9'])) && (isNumber2 xs)

isNumber2 [] = True
isNumber2 (x:xs) = (elem x (['0'..'9'])) && (isNumber2 xs)

getNumber (ASTNode (ASTSimpleDatum x) _ _ ) = x

str_result= ["cat","str"]
num_result= ["plus","times","len","num","negate"]

getFirstError [[]] = ""
getFirstError error_lst
    | x == "" = getFirstError xs
    | otherwise = x
    where x = (head error_lst)  
          xs = (tail error_lst)
    
checkError error_lst EmptyAST = error_lst 
checkError error_lst (ASTNode (ASTSimpleDatum simple) left EmptyAST)
    | simple == "num" = if ( isNumber (getNumber left) ) then error_lst else ("the value '" ++ (getNumber left) ++ "' is not a number!"):error_lst
    | simple == "str" = (checkError error_lst left)
    | simple == "len" = (checkError error_lst left) ++  (if (elem (getNumber left) str_result) then [[]] else ["len operation is not defined on num!"])
    | simple == "negate" = (checkError error_lst left) ++ (if (elem (getNumber left) num_result) then [[]] else ["negate operation is not defined on str!"])
checkError error_lst (ASTNode (ASTLetDatum _) _ _) = error_lst
checkError error_lst (ASTNode (ASTSimpleDatum simple) left right)
    | simple == "plus" = (checkError error_lst left) ++ (checkError error_lst right) ++ (if ( (elem (getNumber left) num_result) && (elem (getNumber right) num_result) ) then [[]] else ["plus operation is not defined between "++(getNumber left)++" and "++(getNumber right)++"!"])
    | simple == "times" = (checkError error_lst left) ++ (checkError error_lst right) ++ (if ( (elem (getNumber left) num_result) && (elem (getNumber right) num_result) ) then [[]] else ["times operation is not defined between "++(getNumber left)++" and "++(getNumber right)++"!"])
    | simple == "cat" = (checkError error_lst left) ++ (checkError error_lst right) ++ (if ( (elem (getNumber left) str_result) && (elem (getNumber right) str_result) ) then [[]] else ["cat operation is not defined between "++(getNumber left)++" and "++(getNumber right)++"!"])
checkError error_lst _ = error_lst

eagerEvaluation x
    | a == "" = if (elem (getNumber b) num_result) then ASTJust ((runEvaluate b),"num",(length (count_ops x))) else ASTJust ((runEvaluate b),"str",(length (count_ops x)))
    | otherwise = (ASTError a )
    where a = (getFirstError (eagerError x))
          b = fst (myNormalEval (x,[]))

eagerError (EmptyAST) = []
eagerError (ASTNode (ASTLetDatum x) left right) = [(getFirstError (checkError [[]] (fst (myNormalEval (left, []))) ) )] ++ [(getFirstError (checkError [[]] (fst (myNormalEval (right, [(x,fst(myNormalEval (left,[])))]))) ) )] ++ [(getFirstError (checkError [[]] (fst (myNormalEval ((ASTNode (ASTLetDatum x) left right),[]) )) ) )]
eagerError (ASTNode (ASTSimpleDatum x) EmptyAST EmptyAST) = []
eagerError (ASTNode (ASTSimpleDatum x) left EmptyAST) = [(getFirstError (checkError [[]] (fst (myNormalEval (left, []))) ) )] ++ [(getFirstError (checkError [[]] (fst (myNormalEval ((ASTNode (ASTSimpleDatum x) left EmptyAST),[]) )) ) )]
eagerError (ASTNode (ASTSimpleDatum x) left right) = [(getFirstError (checkError [[]] (fst (myNormalEval (left, []))) ) )] ++ [(getFirstError (checkError [[]] (fst (myNormalEval (right, []))) ) )] ++ [(getFirstError (checkError [[]] (fst (myNormalEval ((ASTNode (ASTSimpleDatum x) left right),[]) )) ) )]

normalEvaluation x
    | getFirstError (checkError [[]] a) == "" = if (elem (getNumber a) num_result) then ASTJust ((runEvaluate a),"num",(length (count_ops a))) else ASTJust ((runEvaluate a),"str",(length (count_ops a)))
    | otherwise = (ASTError (getFirstError (checkError [[]] a)))
    where a = fst (myNormalEval (x,[]))

count_ops (ASTNode (ASTLetDatum _) left right) = (count_ops left) ++ (count_ops right)
count_ops (ASTNode (ASTSimpleDatum x) EmptyAST EmptyAST)=[]
count_ops (ASTNode (ASTSimpleDatum x) left EmptyAST)
    | ((x=="len") || (x=="negate"))= [1]++(count_ops left)
    | otherwise = (count_ops left)
count_ops (ASTNode (ASTSimpleDatum x) left right)
    | ((x=="plus") || (x=="times") || (x=="cat") ) = (count_ops left) ++ (count_ops right) ++ [1]
    | otherwise = (count_ops left) ++ (count_ops right)

get_if_var_in_lst (var,[]) = (EmptyAST, False)
get_if_var_in_lst (var, (x:xs))
    | (fst x) == var = ( (snd x),True)
    | otherwise = get_if_var_in_lst (var, xs)


myNormalEval (EmptyAST,lst) = (EmptyAST,lst)
myNormalEval ( (ASTNode (ASTSimpleDatum x) EmptyAST EmptyAST), lst)
    | (snd a) = myNormalEval ( (fst a), lst)
    | otherwise = ( (ASTNode (ASTSimpleDatum x) EmptyAST EmptyAST), lst)
    where a = (get_if_var_in_lst (x,lst) )
myNormalEval ( (ASTNode (ASTSimpleDatum x) left EmptyAST),lst) = ( (ASTNode (ASTSimpleDatum x) (fst (myNormalEval (left,lst)) ) EmptyAST),lst)
myNormalEval ( (ASTNode (ASTSimpleDatum x) left right), lst) =   ( (ASTNode (ASTSimpleDatum x) (fst (myNormalEval (left,lst)) ) (fst (myNormalEval (right,lst)) ) ),lst)
myNormalEval (ASTNode (ASTLetDatum x) left right , lst) = myNormalEval (right, (x,left):lst)


-- (ASTNode (ASTLetDatum "x") (ASTNode (ASTLetDatum "y") (ASTNode (ASTSimpleDatum "num") (ASTNode (ASTSimpleDatum "5") EmptyAST EmptyAST) EmptyAST) ( ASTNode (ASTSimpleDatum "y") EmptyAST EmptyAST) ) (ASTNode (ASTSimpleDatum "plus") (ASTNode (ASTSimpleDatum "x") EmptyAST EmptyAST) (ASTNode (ASTSimpleDatum "x") EmptyAST EmptyAST) ))
runEvaluate (ASTNode (ASTSimpleDatum str) left EmptyAST)
    | str == "len" = show (length (runString left ))
    | str == "negate" = show (-1 * (runInt left ) )
    | str == "num" = show (runInt left)
    | str == "str" = (runString left)
runEvaluate (ASTNode (ASTSimpleDatum str) left right)
    | str == "plus" = show ((runInt left) + (runInt right))
    | str == "times" = show ((runInt left) * (runInt right))
    | str == "cat" = (runString left) ++ (runString right)

runInt (ASTNode (ASTSimpleDatum str) EmptyAST EmptyAST) = read str :: Int
runInt (ASTNode (ASTSimpleDatum str) left EmptyAST)
    | str == "num" = (getNum left)
    | str == "negate" = (-1 * (runInt left))
    | str == "len" = (length (runString left))
runInt (ASTNode (ASTSimpleDatum str) left right)
    | str == "plus" = ((runInt left) + (runInt right))
    | str == "times" = ((runInt left) * (runInt right))

runString (ASTNode (ASTSimpleDatum str) EmptyAST EmptyAST) = str
runString (ASTNode (ASTSimpleDatum str) left EmptyAST)
    | str == "str" = (getStr left)
runString (ASTNode (ASTSimpleDatum str) left right)
    | str == "cat" = (runString left) ++ (runString right)

getNum (ASTNode (ASTSimpleDatum str) _ _) = read str :: Int
getStr (ASTNode (ASTSimpleDatum str) _ _) = str

-- (ASTNode (ASTLetDatum "x") (ASTNode (ASTSimpleDatum "num") (ASTNode (ASTSimpleDatum "abc") EmptyAST EmptyAST) EmptyAST) (ASTNode (ASTSimpleDatum "num") (ASTNode (ASTSimpleDatum "5") EmptyAST EmptyAST) EmptyAST))