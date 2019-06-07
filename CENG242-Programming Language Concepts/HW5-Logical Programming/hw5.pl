:- module(hw5, [catomic_number/2, ion/2, molecule/2]).
:- [catoms].

find_catomic_number([], 0).
find_catomic_number([Head | Tail], Sum) :- find_catomic_number(Tail, Tail_sum), Sum is Head+Tail_sum.



catomic_number(NAME,CATOMIC_NUMBER) :- catom( NAME, _, _, LST), find_catomic_number(LST,Res), CATOMIC_NUMBER is Res.


find_last_element([Head | []], Last) :- Last is Head.
find_last_element([_| Tail], Last) :- find_last_element(Tail,Last).

ion(NAME,CHARGE) :- catom(NAME,_,_,LST), 
					find_last_element(LST,Last_Element), 
					(Last_Element>4 -> CHARGE is Last_Element-8;
									  CHARGE is Last_Element).

check_generated_list([],0).
check_generated_list([H|T],Res) :- ion(H,Curr_charge),
									check_generated_list(T,New_charge_sum),
									Res is New_charge_sum + Curr_charge.
									


generate_a_valid_list(X,X,[],Last_taken).

generate_a_valid_list( Total_catom_number, Curr_catom_number,[NAME | T], Last_taken) :- 
	catomic_number(NAME,CATOMIC_NUMBER),
	New_catom_sum is CATOMIC_NUMBER + Curr_catom_number,
	New_catom_sum =< Total_catom_number, 
	Last_taken =< CATOMIC_NUMBER,
	generate_a_valid_list(Total_catom_number,New_catom_sum,T,CATOMIC_NUMBER).

molecule(CATOM_LIST, Total_catom_number) :- 
									generate_a_valid_list(Total_catom_number, 0, CATOM_LIST,0),
									check_generated_list(CATOM_LIST,Charge_sum),
									Charge_sum == 0.