#some parts taken from https://github.com/yuxiamit/DBx1000_logging/blob/master/tools/compile.py
import subprocess
from copy import deepcopy 
import os
import sys
import json
import re

dbms_cfg = ["config-std.h", "config.h"]

def replace(filename, pattern, replacement):
    f = open(filename)
    s = f.read()
    f.close()
    s = re.sub(pattern, replacement, s)
    f = open(filename, 'w')
    f.write(s)
    f.close()

def run_command(command, verbose=False):
    if verbose:
        print("Running: ")
        print(command)
    proc = subprocess.Popen(command, shell=True,
                            stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
    while proc.poll() is None:
        commandResult = proc.wait()  # catch return code
        if commandResult != 0:
            print("Error running: " + command)
            print(proc.stdout.read().decode("ascii"))
            exit(0)
        elif verbose:
            print("Command done!")

def apply_opt_on_file(f):
    command = "cd .. && git checkout -- ./DBx1000/benchmarks/%s && git checkout -- ./DBx1000/storage/row.h" % f
    build_dir = "build"
    if f != "":
        command += " && ./clang-tool/%s/bin/clang-tool ./DBx1000/benchmarks/%s" % (build_dir, f)
    command += " && cd -"
    run_command(command, verbose=True)

def build(conf):
    v = conf['config']
    opt = conf['opt']
    jobname = conf['jobname']

    apply_opt_on_file(opt)

    os.system("cp " + dbms_cfg[0] + ' ' + dbms_cfg[1])
    for (param, value) in v.items():
        pattern = r"\#define\s*" + re.escape(param) + r'.*'
        replacement = "#define " + param + ' ' + str(value)
        replace(dbms_cfg[1], pattern, replacement)

    command = "make clean && make -j && cp rundb exe/rundb_%s && cp config.h exe/rundb_%s.config" % (jobname, jobname)

    print("start to compile " + jobname)
    print(conf)
    run_command(command, verbose=True)

RUNS = 5
def run_job(config):
    jobname = config['jobname']
    print("Running %s" % jobname)

    with open("./data/%s.res" % jobname, "a+") as f:
        f.write('{"conf":')
        conf = config['config']
        conf['opt'] = config['opt']
        f.write(json.dumps(conf))
        f.write(',\n"data":\n [')

    sep = "-s"
    for i in range(RUNS):
        print("Run %i of %i" % (i, RUNS))
        if i == RUNS - 1:
            sep = ""
        run_command("./exe/rundb_%s | python3 scripts/parse_out.py %s >> ./data/%s.res" % (jobname, sep, jobname))

    with open("./data/%s.res" % jobname, "a+") as f:
        f.write(']}')

    print("Done")

def collect_result(jobname, outname="results", header=False):
    flag = ''
    if header:
        flag = '-h'
    run_command('python3 scripts/out_to_csv.py %s %s >> %s.csv' % (jobname, flag, outname))
    
init_configs = [
    {"config": {"CC_ALG" : "TIMESTAMP", "WORKLOAD" : "SYNTH", "SYNTH_PRIME": "10","THREAD_CNT": "8", "PERC_PAYMENT": "0.5", "SYNTH_ROWS" : "100000", "SYNTH_DIST": "SYNTH_ZIPF", "ZIPF_THETA": "0.6"}, "opt": "synth_txn.cpp", 'jobname': "timestamp_opt"},
    {"config": {"CC_ALG" : "TIMESTAMP", "WORKLOAD" : "SYNTH", "SYNTH_PRIME": "10","THREAD_CNT": "8", "PERC_PAYMENT": "0.5", "SYNTH_ROWS" : "100000", "SYNTH_DIST": "SYNTH_ZIPF", "ZIPF_THETA": "0.6"}, "opt": "",              'jobname': 'timestamp'},
    {"config": {"CC_ALG" : "DL_DETECT", "WORKLOAD" : "SYNTH", "SYNTH_PRIME": "10","THREAD_CNT": "8", "PERC_PAYMENT": "0.5", "SYNTH_ROWS" : "100000", "SYNTH_DIST": "SYNTH_ZIPF", "ZIPF_THETA": "0.6"}, "opt": "",              'jobname': 'dl_detect'},
    {"config": {"CC_ALG" : "NO_WAIT",   "WORKLOAD" : "SYNTH", "SYNTH_PRIME": "10","THREAD_CNT": "8", "PERC_PAYMENT": "0.5", "SYNTH_ROWS" : "100000", "SYNTH_DIST": "SYNTH_ZIPF", "ZIPF_THETA": "0.6"}, "opt": "",              'jobname': 'no_wait'},
    {"config": {"CC_ALG" : "OCC",       "WORKLOAD" : "SYNTH", "SYNTH_PRIME": "10","THREAD_CNT": "8", "PERC_PAYMENT": "0.5", "SYNTH_ROWS" : "100000", "SYNTH_DIST": "SYNTH_ZIPF", "ZIPF_THETA": "0.6"}, "opt": "",              'jobname': 'occ'},
    {"config": {"CC_ALG" : "MVCC",      "WORKLOAD" : "SYNTH", "SYNTH_PRIME": "10","THREAD_CNT": "8", "PERC_PAYMENT": "0.5", "SYNTH_ROWS" : "100000", "SYNTH_DIST": "SYNTH_ZIPF", "ZIPF_THETA": "0.6"}, "opt": "",              'jobname': 'mvcc'},
  ]

def gen_rdwr_configs(prefix, zipf_theta):
    new_configs = []
    perc = 0
    while perc <= 1:
        for config in init_configs:
            new_config = deepcopy(config)
            new_config['config']['PERC_PAYMENT'] = perc
            new_config['config']['ZIPF_THETA'] = zipf_theta
            new_config['jobname'] = new_config['jobname'] + "_" + prefix  + "_" + str(perc)
            new_configs.append(new_config)
        perc += 0.2
    return new_configs


def gen_thread_configs(prefix, zipf_theta):
    new_configs = []
    for threads in ['8', '12', '26', '52']:
        for config in init_configs:
            new_config = deepcopy(config)
            new_config['config']['THREAD_CNT'] = threads
            new_config['config']['ZIPF_THETA'] = zipf_theta
            new_config['jobname'] = new_config['jobname'] + "_" + prefix + "_" + str(threads)
            new_configs.append(new_config)
    return new_configs


def gen_slow_configs(prefix, zipf_theta):
    new_configs = []
    for prime in range(10, 120, 20):
        for config in init_configs:
            new_config = deepcopy(config)
            new_config['config']['SYNTH_PRIME'] = prime
            new_config['config']['ZIPF_THETA'] = zipf_theta
            new_config['jobname'] = new_config['jobname'] + "_" + prefix + "_" + str(prime)
            new_configs.append(new_config)
    return new_configs


experiments  = {
               'rdwr_perc_high': gen_rdwr_configs("high_perc", "0.8"),
               'rdwr_perc_low': gen_rdwr_configs("low_perc", "0.1"),
               'threads_high': gen_thread_configs("high_threads", "0.8"),
               'threads_low': gen_thread_configs("low_threads", "0.1"),
               'prime_high': gen_slow_configs("high_prime", "0.8"),
               'prime_low': gen_slow_configs("low_prime", "0.1"),
               }
    
if __name__ == "__main__":
    run_command("rm -rf data exe")
    run_command("mkdir -p exe")
    run_command("mkdir -p data")

    build_flag = True
    if len(sys.argv) > 1 and sys.argv[1] == '-nobuild':
        build_flag=False
    
    for experiment, configs in experiments.items():
        header = True
        for conf in configs:
            if build_flag:
                build(conf)
            run_job(conf)
            jobname = conf['jobname']
            collect_result(jobname, header=header, outname=experiment)
            header = False
