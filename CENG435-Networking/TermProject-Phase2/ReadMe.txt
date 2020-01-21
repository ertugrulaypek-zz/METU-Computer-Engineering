#Group 25
#Özgürcan Çalışkan 2171460
#Ertuğrul Aypek 2171270
###########################################################################


How to run after initializing given Topology:

Copy experiment.py script to all nodes.

Copy configureR1.sh, configureR2.sh, configureR3.sh files to r1,r2 and r3 respectively.

When trying to run them, DOS line endings that cause the problem. To fix, open the file in Vim and run
:set ff=unix
:wq

Then, run configureR1.sh, configureR2.sh, configureR3.sh files on r1,r2,r3 respectively
sh configureR1.sh	#node r1
sh configureR2.sh	#node r2
sh configureR3.sh	#node r3

Firstly run experiment.py on r1,r2,r3 and d respectively with command line argument such that
python experiment.py r1		#node r1
python experiment.py r2		#node r2
python experiment.py r3		#node r3
python experiment.py d		#node d


Then run it on s with command line argument such that
python experiment.py s		#node s  #It transmits file for both experiments once at the same time by multi threading. 

To find standart error, run it on s with command line argument such that
python experiment.py sExp       #node s  #It transmits file for experiment2 n times and calculates avg and error. 


###############################################################################

We used "python experiment.py sExp" for report to get avg and standart error. Moreover, we used to "python experiment.py s" in order to transmit input.txt file to destination for both experiment. After "python experiment.py s" completed, in the destination, there will be two files named input1.txt and input2.txt. input1.txt is for experiment 1 and input2.txt is for experiment2. Then, original input file can be sent to destination by using scp. After that, we test their similarities with linux 'diff' command such that

diff input1.txt input.txt     	#to check correctness of experiment 1
diff input2.txt input.txt	#to check correctness of experiment 2


