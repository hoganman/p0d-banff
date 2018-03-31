import qsubmitter2
import getopt
import sys

ft_jobs = qsubmitter2.flattree_jobs()
shortArgs = ft_jobs.make_getopt_options()
longArgsList = ft_jobs.make_getopt_long_options()
argv = '-o name.root -n 2'.split()
try:
    opts, args = getopt.getopt(argv, shortArgs, longArgsList)
except getopt.GetoptError as optErr:
    print "could not find " + str(optErr)
    ft_jobs.get_option('help').usr_input = True
    print ft_jobs.help_msg()
    sys.exit(1)
ft_jobs.set_usr_inputs_from_getopt(opts, args)
print ft_jobs.get_option('-n').usr_input
print ft_jobs.get_option('-o').usr_input
ft_jobs.set_qsubDir()
if ft_jobs.qsubDir.exists():
    ft_jobs.qsubDir.cd()
else:
    sys.exit(1)
ft_jobs.make_qsub_script('test.txt')
sys.exit(0)
