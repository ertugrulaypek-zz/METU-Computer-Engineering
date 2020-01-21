How to run after initializing given Topology and configure files:

Copy discovery.py script to all nodes.

Firstly, run it on s and d respectively with command line argument such that
python discovery.py s
python discovery.py d

Then run it on r2 with command line argument such that
python discovery.py r2

Then run it on r1 and r3 respectively with command line argument such that
python discovery.py r1
python discovery.py r3

After all script ends, cost_link.txt files can be found in r1,r2 and r3.
