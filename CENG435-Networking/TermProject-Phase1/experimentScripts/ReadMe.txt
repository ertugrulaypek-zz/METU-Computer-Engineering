How to run after initializing given Topology and configure files:

Copy experiment.py script to all nodes.
Copy configureR3_1.sh, configureR3_2.sh, configureR3_3.sh files to r3.
When trying to run them, DOS line endings that cause the problem. To fix, open the file in Vim and run
:set ff=unix
:wq

Then, run configureR3_1.sh file on r3 such that
sh configureR3_1.sh

Firstly run experiment.py on d and r3 respectively with command line argument such that
python experiment.py d
python experiment.py r3

Then run it on s with command line argument such that
python experiment.py s

After all script ends, e2eDelay.txt files can be found in s.

After that change configuration and again run experiment script in same order.
sh configureR3_2.sh, sh configureR3_3.sh will change the configuration.

